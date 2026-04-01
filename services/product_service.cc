#include <utility>
#include <stdexcept>
#include <vector>
#include <optional>
#include <string_view>
#include <string>
#include <ranges>
#include "product_service.h"

#include <shared_mutex>
#include <drogon/drogon.h>
#include <glaze/glaze.hpp>

#include <repositories/repo.h>
#include <dto/dto.h>
#include <domains/domain.h>

using namespace drogon;

namespace {
// Cache stores serialized JSON strings for extreme performance
std::unordered_map<std::string, std::string> product_cache_json;
// Pre-serialized array of all active products
std::string all_products_json_cache{"[]"};
std::shared_mutex cache_mutex;

/**
 * @brief Helper to convert domain to DTO. 
 * Includes images fetched from repository.
 */
[[nodiscard]] dto::ProductResponse ToProductResponse(const domain::Product& product, const std::vector<dto::ProductImageResponse>& images) {
  dto::ProductResponse response;
  response.id = product.getId();
  response.type = product.getType();
  response.name = product.getName();
  response.price = product.getPrice();
  response.area = product.getArea();
  response.description = product.getDescription();
  response.is_active = product.getIsActive();
  response.created_at = product.getCreatedAt();
  response.updated_at = product.getUpdatedAt();
  response.images = images;
  return response;
}
}

namespace {
// Store DTOs to allow easy rebuilding of the global JSON array
std::unordered_map<std::string, dto::ProductResponse> product_dto_cache;

void RebuildAllJson() {
  auto active_products{product_dto_cache | std::views::values 
                         | std::views::filter([](const auto& product){ return product.is_active && *product.is_active; })};
  
  std::vector<dto::ProductResponse> vec;
  std::ranges::copy(active_products, std::back_inserter(vec));
  
  auto json{glz::write_json(vec)};
  all_products_json_cache = std::move(json).value_or("[]");
}
}

drogon::Task<void> service::product::InitializeCache() {
  LOG_INFO << "Initializing product cache...";
  auto products_raw{co_await repo::product::FindAll()};
  
  std::unique_lock lock{cache_mutex};
  product_cache_json.clear();
  product_dto_cache.clear();

  for (auto& product_domain : products_raw) {
    const std::string product_id{*product_domain.getId()};
    auto images{co_await repo::product_images::FindByProductId(product_id)};
    
    // Convert to DTO
    std::vector<dto::ProductImageResponse> img_dtos;
    for(const auto& image : images) {
        dto::ProductImageResponse ir;
        ir.id = image.getId();
        ir.product_id = image.getProductId();
        ir.url = image.getUrl();
        ir.is_cover = image.getIsCover();
        ir.sort_order = image.getSortOrder();
        ir.uploaded_by = image.getUploadedBy();
        ir.created_at = image.getCreatedAt();
        img_dtos.push_back(std::move(ir));
    }

    auto response{ToProductResponse(product_domain, img_dtos)};
    auto json{glz::write_json(response)};
    
    product_cache_json[product_id] = std::move(json).value_or("{}");
    product_dto_cache[product_id] = std::move(response);
  }
  
  RebuildAllJson();
  LOG_INFO << "Product cache initialized with " << product_cache_json.size() << " items.";
}

drogon::Task<void> service::product::RefreshCache(std::string product_id) {
  auto product_opt{co_await repo::product::FindById(product_id)};
  if (!product_opt) {
    std::unique_lock lock{cache_mutex};
    product_cache_json.erase(product_id);
    product_dto_cache.erase(product_id);
    RebuildAllJson();
    co_return;
  }

  auto images{co_await repo::product_images::FindByProductId(product_id)};
  std::vector<dto::ProductImageResponse> img_dtos;
  for(const auto& image : images) {
      dto::ProductImageResponse ir;
      ir.id = image.getId();
      ir.product_id = image.getProductId();
      ir.url = image.getUrl();
      ir.is_cover = image.getIsCover();
      ir.sort_order = image.getSortOrder();
      ir.uploaded_by = image.getUploadedBy();
      ir.created_at = image.getCreatedAt();
      img_dtos.push_back(std::move(ir));
  }

  auto response{ToProductResponse(*product_opt, img_dtos)};
  auto json{glz::write_json(response)};

  {
    std::unique_lock lock{cache_mutex};
    product_cache_json[product_id] = std::move(json).value_or("{}");
    product_dto_cache[product_id] = std::move(response);
    RebuildAllJson();
  }
}

drogon::Task<dto::ProductResponse> service::product::Create(
    dto::CreateProductRequest request, std::vector<std::string> new_image_urls) {
  domain::Product product;
  product.setType(std::move(request.type));
  product.setName(std::move(request.name));
  product.setPrice(std::move(request.price));
  if (request.area) product.setArea(std::move(*request.area));
  if (request.description) product.setDescription(std::move(*request.description));
  product.setIsActive(request.is_active);

  auto created{co_await repo::product::Create(std::move(product))};
  
  // Update cache
  co_await RefreshCache(*created.getId());
  
  co_return product_dto_cache[*created.getId()];
}

drogon::Task<void> service::product::Update(
    std::string product_id, dto::UpdateProductRequest request, std::vector<std::string> new_image_urls) {
  auto product_opt{co_await repo::product::FindById(product_id)};
  if (!product_opt) {
    throw std::runtime_error{"Product not found"};
  }

  auto existing{std::move(*product_opt)};

  if (request.type) existing.setType(std::move(*request.type));
  if (request.name) existing.setName(std::move(*request.name));
  if (request.price) existing.setPrice(std::move(*request.price));
  if (request.area) existing.setArea(std::move(*request.area));
  if (request.description) existing.setDescription(std::move(*request.description));
  if (request.is_active) existing.setIsActive(*request.is_active);

  co_await repo::product::Update(existing);
  
  co_await RefreshCache(product_id);
}

drogon::Task<void> service::product::Delete(std::string product_id) {
  auto deleted{co_await repo::product::DeleteById(product_id)};
  if (deleted) {
    std::unique_lock lock{cache_mutex};
    product_cache_json.erase(product_id);
    product_dto_cache.erase(product_id);
    RebuildAllJson();
  }
}

std::optional<std::string> service::product::GetByIdJson(
    std::string_view product_id) {
  std::shared_lock lock{cache_mutex};
  if (auto it{product_cache_json.find(std::string(product_id))}; it != product_cache_json.end()) {
    return it->second;
  }
  return std::nullopt;
}

std::string service::product::GetAllJson() {
  std::shared_lock lock{cache_mutex};
  return all_products_json_cache;
}

std::vector<dto::ProductResponse> service::product::Search(
    dto::ProductSearchQuery query) {
  std::shared_lock lock{cache_mutex};
  
  auto filtered_view{product_dto_cache | std::views::values 
                       | std::views::filter([&query](const dto::ProductResponse& product) {
    if (query.name && product.name && product.name->find(*query.name) == std::string::npos) return false;
    if (query.type && product.type && *product.type != *query.type) return false;
    if (query.min_price && product.price && std::stod(*product.price) < std::stod(*query.min_price)) return false;
    if (query.max_price && product.price && std::stod(*product.price) > std::stod(*query.max_price)) return false;
    return true;
  })};

  std::vector<dto::ProductResponse> result;
  std::ranges::copy(filtered_view, std::back_inserter(result));
  return result;
}
