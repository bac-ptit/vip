#pragma once
#include "dto/product_images_dto.h"
#include <drogon/HttpRequest.h>
#include <glaze/glaze.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <trantor/utils/Date.h>
#include <vector>

namespace dto {

struct CreateProductRequest {
  std::string type;
  std::string name;
  std::string price;
  std::optional<std::string> area;
  std::optional<std::string> description;
  bool is_active{true};
};

struct UpdateProductRequest {
  std::optional<std::string> type;
  std::optional<std::string> name;
  std::optional<std::string> price;
  std::optional<std::string> area;
  std::optional<std::string> description;
  std::optional<bool> is_active;
};

struct ProductResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> type;
  std::shared_ptr<std::string> name;
  std::shared_ptr<std::string> price;
  std::shared_ptr<std::string> area;
  std::shared_ptr<std::string> description;
  std::shared_ptr<bool> is_active;
  std::shared_ptr<trantor::Date> created_at;
  std::shared_ptr<trantor::Date> updated_at;
  std::vector<ProductImageResponse> images;
};

struct ProductSearchQuery {
  std::optional<std::string> name;
  std::optional<std::string> type;
  std::optional<std::string> min_price;
  std::optional<std::string> max_price;
};

}  // namespace dto

namespace drogon {

template <>
inline dto::CreateProductRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::CreateProductRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for CreateProductRequest"};
  }
  return request;
}

template <>
inline dto::UpdateProductRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateProductRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateProductRequest"};
  }
  return request;
}

}  // namespace drogon
