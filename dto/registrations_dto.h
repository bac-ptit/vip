#pragma once
#include <drogon/HttpRequest.h>
#include <glaze/glaze.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <trantor/utils/Date.h>
#include <vector>

namespace dto {

struct CreateRegistrationRequest {
  std::string full_name;
  std::string phone;
  std::optional<std::string> email;
  std::optional<std::string> address;
  std::optional<std::string> note;
  std::vector<std::string> interests;
};

struct UpdateRegistrationRequest {
  std::optional<bool> is_contacted;
  std::optional<std::string> note;
};

struct RegistrationResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> full_name;
  std::shared_ptr<std::string> phone;
  std::shared_ptr<std::string> email;
  std::shared_ptr<std::string> address;
  std::shared_ptr<std::string> note;
  std::shared_ptr<bool> is_contacted;
  std::shared_ptr<std::string> contacted_by;
  std::shared_ptr<trantor::Date> contacted_at;
  std::shared_ptr<trantor::Date> created_at;
  std::shared_ptr<std::vector<std::string>> interests;
};

}  // namespace dto

namespace drogon {

template <>
inline dto::CreateRegistrationRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::CreateRegistrationRequest request;

  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for CreateRegistrationRequest"};
  }

  if (request.full_name.empty() || request.phone.empty()) {
    throw std::runtime_error{
        "Missing required fields: full_name and phone are mandatory"};
  }

  return request;
}

template <>
inline dto::UpdateRegistrationRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateRegistrationRequest request;

  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateRegistrationRequest"};
  }

  return request;
}

}  // namespace drogon
