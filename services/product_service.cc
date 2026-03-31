//
// Created by bac on 3/27/26.
//
module;
#include <drogon/drogon.h>
#include <glaze/glaze.hpp>
#include <mutex>
#include <optional>
#include <ranges>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

module service;

import repo;
import dto;
import domain;

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
[[nodiscard]] dto::ProductResponse ToProductResponse(const domain::Product& p, const std::vector<dto::ProductImageResponse>& images) {
  dto::ProductResponse resp;
  resp.id = p.getId();
  resp.type = p.getType();
  resp.name = p.getName();
  resp.price = p.getPrice();
  resp.area = p.getArea();
  resp.description = p.getDescription();
  resp.is_active = p.getIsActive();
  resp.created_at = p.getCreatedAt();
  resp.updated_at = p.getUpdatedAt();
  resp.images = images;
  return resp;
}

/**
 * @brief Rebuilds the global JSON array cache.
 * Called whenever individual product cache changes.
 */
void RebuildGlobalCache() {
  std::vector<std::string_view> active_json_views;
  // We need to actually parse back to list or maintain a list of DTOs to rebuild
  // Alternatively, just concatenate strings with commas if we want to stay string-only
  // but simpler to just keep a list of DTOs for rebuilding the big one.
}
}

namespace {
// Store DTOs to allow easy rebuilding of the global JSON array
std::unordered_map<std::string, dto::ProductResponse> product_dto_cache;

void RebuildAllJson() {
  auto active_products = product_dto_cache | std::views::values 
                         | std::views::filter([](const auto& p){ return p.is_active && *p.is_active; });
  
  std::vector<dto::ProductResponse> vec;
  std::ranges::copy(active_products, std::back_inserter(vec));
  
  auto json {glz::write_json(vec)};
  all_products_json_cache = std::move(json).value_or("[]");
}
}

drogon::Task<void> service::product::InitializeCache() {
  LOG_INFO << "Initializing product cache...";
  auto products_raw{co_await repo::product::FindAll()};
  
  std::unique_lock lock{cache_mutex};
  product_cache_json.clear();
  product_dto_cache.clear();

  for (auto&& p_domain : products_raw) {
    const std::string id = *p_domain.getId();
    auto images = co_await repo::product_images::FindByProductId(id);
    
    // Convert to DTO
    std::vector<dto::ProductImageResponse> img_dtos;
    for(const auto& img : images) {
        dto::ProductImageResponse ir;
        ir.id = img.getId();
        ir.product_id = img.getProductId();
        ir.url = img.getUrl();
        ir.is_cover = img.getIsCover();
        ir.sort_order = img.getSortOrder();
        ir.uploaded_by = img.getUploadedBy();
        ir.created_at = img.getCreatedAt();
        img_dtos.push_back(std::move(ir));
    }

    auto resp = ToProductResponse(p_domain, img_dtos);
    auto json = glz::write_json(resp);
    
    product_cache_json[id] = std::move(json).value_or("{}");
    product_dto_cache[id] = std::move(resp);
  }
  
  RebuildAllJson();
  LOG_INFO << "Product cache initialized with " << product_cache_json.size() << " items.";
}

drogon::Task<void> service::product::RefreshCache(std::string product_id) {
  auto p_opt = co_await repo::product::FindById(product_id);
  if (!p_opt) {
    std::unique_lock lock{cache_mutex};
    product_cache_json.erase(product_id);
    product_dto_cache.erase(product_id);
    RebuildAllJson();
    co_return;
  }

  auto images = co_await repo::product_images::FindByProductId(product_id);
  std::vector<dto::ProductImageResponse> img_dtos;
  for(const auto& img : images) {
      dto::ProductImageResponse ir;
      ir.id = img.getId();
      ir.product_id = img.getProductId();
      ir.url = img.getUrl();
      ir.is_cover = img.getIsCover();
      ir.sort_order = img.getSortOrder();
      ir.uploaded_by = img.getUploadedBy();
      ir.created_at = img.getCreatedAt();
      img_dtos.push_back(std::move(ir));
  }

  auto resp = ToProductResponse(*p_opt, img_dtos);
  auto json = glz::write_json(resp);

  {
    std::unique_lock lock{cache_mutex};
    product_cache_json[product_id] = std::move(json).value_or("{}");
    product_dto_cache[product_id] = std::move(resp);
    RebuildAllJson();
  }
}

drogon::Task<dto::ProductResponse> service::product::Create(
    dto::CreateProductRequest request) {
  domain::Product p;
  p.setType(std::move(request.type));
  p.setName(std::move(request.name));
  p.setPrice(std::move(request.price));
  if (request.area) p.setArea(std::move(*request.area));
  if (request.description) p.setDescription(std::move(*request.description));
  p.setIsActive(request.is_active);

  auto created{co_await repo::product::Create(std::move(p))};
  
  // Update cache
  co_await RefreshCache(*created.getId());
  
  co_return product_dto_cache[*created.getId()];
}

drogon::Task<void> service::product::Update(
    std::string id, dto::UpdateProductRequest request) {
  auto existing_opt{co_await repo::product::FindById(id)};
  if (!existing_opt) {
    throw std::runtime_error{"Product not found"};
  }

  auto existing{std::move(*existing_opt)};

  if (request.type) existing.setType(std::move(*request.type));
  if (request.name) existing.setName(std::move(*request.name));
  if (request.price) existing.setPrice(std::move(*request.price));
  if (request.area) existing.setArea(std::move(*request.area));
  if (request.description) existing.setDescription(std::move(*request.description));
  if (request.is_active) existing.setIsActive(*request.is_active);

  co_await repo::product::Update(existing);
  
  co_await RefreshCache(id);
}

drogon::Task<void> service::product::Delete(std::string id) {
  auto deleted{co_await repo::product::DeleteById(id)};
  if (deleted) {
    std::unique_lock lock{cache_mutex};
    product_cache_json.erase(id);
    product_dto_cache.erase(id);
    RebuildAllJson();
  }
}

std::optional<std::string> service::product::GetByIdJson(
    std::string_view id) {
  std::shared_lock lock{cache_mutex};
  if (auto it{product_cache_json.find(std::string(id))}; it != product_cache_json.end()) {
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
  
  auto filtered_view = product_dto_cache | std::views::values 
                       | std::views::filter([&query](const dto::ProductResponse& p) {
    if (query.name && p.name && p.name->find(*query.name) == std::string::npos) return false;
    if (query.type && p.type && *p.type != *query.type) return false;
    if (query.min_price && p.price && std::stod(*p.price) < std::stod(*query.min_price)) return false;
    if (query.max_price && p.price && std::stod(*p.price) > std::stod(*query.max_price)) return false;
    return true;
  });

  std::vector<dto::ProductResponse> result;
  std::ranges::copy(filtered_view, std::back_inserter(result));
  return result;
}
