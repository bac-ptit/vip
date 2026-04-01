#pragma once
#include <optional>
#include <string>
//
// Created by bac on 3/28/26.
//


#include <drogon/drogon.h>
#include <glaze/glaze.hpp>
#include <trantor/utils/Date.h>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>




namespace dto {

struct CreateFloorPlanRequest {
  std::string title;
  std::string url;
  short sort_order{0};
  bool is_active{true};
};

struct UpdateFloorPlanRequest {
  std::optional<std::string> title;
  std::optional<std::string> url;
  std::optional<short> sort_order;
  std::optional<bool> is_active;
};

struct FloorPlanResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> title;
  std::shared_ptr<std::string> url;
  std::shared_ptr<short> sort_order;
  std::shared_ptr<bool> is_active;
  std::shared_ptr<std::string> uploaded_by;
  std::shared_ptr<trantor::Date> created_at;
};

}  // namespace dto

namespace drogon {

template <>
inline dto::CreateFloorPlanRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::CreateFloorPlanRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for CreateFloorPlanRequest"};
  }
  return request;
}

template <>
inline dto::UpdateFloorPlanRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateFloorPlanRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateFloorPlanRequest"};
  }
  return request;
}

}  // namespace drogon
