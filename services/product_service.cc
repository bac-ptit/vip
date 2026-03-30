//
// Created by bac on 3/27/26.
//
module;
#include <drogon/drogon.h>
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
std::unordered_map<std::string, dto::ProductResponse> product_cache;
std::vector<dto::ProductResponse> product_cache_vec;  // thêm cái này
std::shared_mutex cache_mutex;

void RebuildVec() {  // gọi sau mỗi write
  product_cache_vec.clear();
  product_cache_vec.reserve(product_cache.size());
  for (auto& [k, v] : product_cache) {
    product_cache_vec.push_back(v);
  }
}
}

namespace {
// Thread-safe in-memory cache
dto::ProductResponse ToProductResponse(const domain::Product& p) {
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
  return resp;
}
}  // namespace

drogon::Task<void> service::product::InitializeCache() {
  auto products_raw{co_await repo::product::FindAll()};
  std::unique_lock lock{cache_mutex};
  product_cache.clear();
  for (auto&& p : products_raw) {
    auto p_domain{std::move(p)};
    product_cache.emplace(*p_domain.getId(), ToProductResponse(p_domain));
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
  auto resp{ToProductResponse(created)};

  {
    std::unique_lock lock{cache_mutex};
    product_cache.emplace(*resp.id, resp);
  }
  co_return resp;
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
  auto updated_resp{ToProductResponse(existing)};

  {
    std::unique_lock lock{cache_mutex};
    product_cache[std::move(id)] = updated_resp;
  }
}

drogon::Task<void> service::product::Delete(std::string id) {
  auto deleted{co_await repo::product::DeleteById(id)};
  if (deleted) {
    std::unique_lock lock{cache_mutex};
    product_cache.erase(std::move(id));
  }
}

std::optional<dto::ProductResponse> service::product::GetById(
    std::string id) {
  std::shared_lock lock{cache_mutex};
  if (auto it{product_cache.find(id)}; it != product_cache.end()) {
    return it->second;
  }
  return std::nullopt;
}

std::vector<dto::ProductResponse> service::product::GetAll() {
  std::shared_lock lock{cache_mutex};
  return product_cache_vec;  // zero copy, zero alloc
}

std::vector<dto::ProductResponse> service::product::Search(
    dto::ProductSearchQuery query) {
  std::shared_lock lock{cache_mutex};
  
  auto filtered_view{product_cache | std::views::values | std::views::filter([&query](const dto::ProductResponse& p) {
    if (query.name && p.name && p.name->find(*query.name) == std::string::npos) return false;
    if (query.type && p.type && *p.type != *query.type) return false;
    if (query.min_price && p.price && std::stod(*p.price) < std::stod(*query.min_price)) return false;
    if (query.max_price && p.price && std::stod(*p.price) > std::stod(*query.max_price)) return false;
    return true;
  })};

  std::vector<dto::ProductResponse> result;
  std::ranges::copy(filtered_view, std::back_inserter(result));
  return result;
}
