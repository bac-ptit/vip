#pragma once
#include <optional>
#include <string>
//
// Created by bac on 3/28/26.
//


#include <drogon/drogon.h>
#include <glaze/glaze.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>



namespace dto {

struct CreateProjectInfoRequest {
  std::string name;
  std::optional<std::string> developer;
  std::optional<std::string> scale;
  std::optional<std::string> description;
  std::optional<double> map_lat;
  std::optional<double> map_lng;
  short map_zoom{15};
};

struct UpdateProjectInfoRequest {
  std::optional<std::string> name;
  std::optional<std::string> developer;
  std::optional<std::string> scale;
  std::optional<std::string> description;
  std::optional<double> map_lat;
  std::optional<double> map_lng;
  std::optional<short> map_zoom;
};

struct ProjectInfoResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> name;
  std::shared_ptr<std::string> developer;
  std::shared_ptr<std::string> scale;
  std::shared_ptr<std::string> description;
  std::shared_ptr<double> map_lat;
  std::shared_ptr<double> map_lng;
  std::shared_ptr<short> map_zoom;
  std::shared_ptr<std::string> updated_by;
  std::shared_ptr<trantor::Date> updated_at;
};

}  // namespace dto

namespace drogon {

template <>
inline dto::CreateProjectInfoRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::CreateProjectInfoRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for CreateProjectInfoRequest"};
  }
  return request;
}

template <>
inline dto::UpdateProjectInfoRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateProjectInfoRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateProjectInfoRequest"};
  }
  return request;
}

}  // namespace drogon
