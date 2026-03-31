#pragma once
#include <drogon/HttpRequest.h>
#include <glaze/glaze.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <trantor/utils/Date.h>

namespace dto {

struct CreateMediaRequest {
  std::string type;
  std::optional<std::string> title;
  std::string url;
  std::optional<std::string> thumbnail;
  short sort_order{0};
  bool is_active{true};
};

struct UpdateMediaRequest {
  std::optional<std::string> type;
  std::optional<std::string> title;
  std::optional<std::string> url;
  std::optional<std::string> thumbnail;
  std::optional<short> sort_order;
  std::optional<bool> is_active;
};

struct MediaResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> type;
  std::shared_ptr<std::string> title;
  std::shared_ptr<std::string> url;
  std::shared_ptr<std::string> thumbnail;
  std::shared_ptr<short> sort_order;
  std::shared_ptr<bool> is_active;
  std::shared_ptr<std::string> uploaded_by;
  std::shared_ptr<trantor::Date> created_at;
};

}  // namespace dto

namespace drogon {

template <>
inline dto::CreateMediaRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::CreateMediaRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for CreateMediaRequest"};
  }
  return request;
}

template <>
inline dto::UpdateMediaRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateMediaRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateMediaRequest"};
  }
  return request;
}

}  // namespace drogon
