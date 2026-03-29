//
// Created by bac on 3/28/26.
//
module;
#include <drogon/drogon.h>
#include <models/ProductImages.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

module service;

import repo;
import dto;
import domain;

using namespace drogon;

namespace {
dto::ProductImageResponse ToProductImageResponse(const domain::ProductImages& p) {
  dto::ProductImageResponse resp;
  resp.id = p.getId();
  resp.product_id = p.getProductId();
  resp.url = p.getUrl();
  resp.is_cover = p.getIsCover();
  resp.sort_order = p.getSortOrder();
  resp.uploaded_by = p.getUploadedBy();
  resp.created_at = p.getCreatedAt();
  return resp;
}
}  // namespace

drogon::Task<dto::ProductImageResponse> service::product_images::Create(
    dto::CreateProductImageRequest request) {
  domain::ProductImages p;
  p.setProductId(std::move(request.product_id));
  p.setUrl(std::move(request.url));
  p.setIsCover(request.is_cover);
  p.setSortOrder(request.sort_order);

  auto created{co_await repo::product_images::Create(p)};
  co_return ToProductImageResponse(created);
}

drogon::Task<void> service::product_images::Update(
    std::string_view id, dto::UpdateProductImageRequest request) {
  auto existing_opt{co_await repo::product_images::FindById(id)};
  if (!existing_opt) {
    throw std::runtime_error{"Product image not found"};
  }

  auto existing{std::move(*existing_opt)};
  if (request.product_id) existing.setProductId(std::move(*request.product_id));
  if (request.url) existing.setUrl(std::move(*request.url));
  if (request.is_cover) existing.setIsCover(*request.is_cover);
  if (request.sort_order) existing.setSortOrder(*request.sort_order);

  co_await repo::product_images::Update(existing);
}

drogon::Task<void> service::product_images::Delete(std::string_view id) {
  co_await repo::product_images::DeleteById(id);
}

drogon::Task<std::optional<dto::ProductImageResponse>> service::product_images::GetById(
    std::string_view id) {
  auto p{co_await repo::product_images::FindById(id)};
  if (p) {
    co_return ToProductImageResponse(*p);
  }
  co_return std::nullopt;
}

drogon::Task<std::vector<dto::ProductImageResponse>> service::product_images::GetByProductId(
    std::string_view product_id) {
  auto images{co_await repo::product_images::FindByProductId(product_id)};
  std::vector<dto::ProductImageResponse> result;
  result.reserve(images.size());
  for (auto& img : images) {
    result.push_back(ToProductImageResponse(img));
  }
  co_return result;
}

drogon::Task<std::vector<dto::ProductImageResponse>> service::product_images::GetActiveByProductId(
    std::string_view product_id) {
  co_return co_await GetByProductId(product_id);
}
