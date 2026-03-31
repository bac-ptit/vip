//
// Created by bac on 3/28/26.
//

module;
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <glaze/glaze.hpp>
export module dto:product_images;
import std;



export namespace dto {

struct CreateProductImageRequest {
  std::string product_id;
  std::string url;
  bool is_cover{false};
  short sort_order{0};
};

struct UpdateProductImageRequest {
  std::optional<std::string> product_id;
  std::optional<std::string> url;
  std::optional<bool> is_cover;
  std::optional<short> sort_order;
};

struct ProductImageResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> product_id;
  std::shared_ptr<std::string> url;
  std::shared_ptr<bool> is_cover;
  std::shared_ptr<short> sort_order;
  std::shared_ptr<std::string> uploaded_by;
  std::shared_ptr<trantor::Date> created_at;
};

}  // namespace dto

namespace drogon {

template <>
inline dto::CreateProductImageRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::CreateProductImageRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for CreateProductImageRequest"};
  }
  return request;
}

template <>
inline dto::UpdateProductImageRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateProductImageRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateProductImageRequest"};
  }
  return request;
}

}  // namespace drogon
