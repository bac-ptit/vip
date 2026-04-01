#pragma once
#include <optional>
#include <string>
//
// Created by bac on 3/27/26.
//


#include <drogon/drogon.h>
#include <glaze/glaze.hpp>
#include <trantor/utils/Date.h>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>



namespace dto {

struct CreateRegistrationInterestRequest {
  std::string registration_id;
  std::string product_type;
};

struct UpdateRegistrationInterestRequest {
  std::optional<std::string> product_type;
};

struct RegistrationInterestResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> registration_id;
  std::shared_ptr<std::string> product_type;
};

}  // namespace dto

namespace drogon {

template <>
inline dto::CreateRegistrationInterestRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::CreateRegistrationInterestRequest request;
  
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for CreateRegistrationInterestRequest"};
  }
  
  if (request.registration_id.empty() || request.product_type.empty()) {
    throw std::runtime_error{"Missing required fields: registration_id and product_type are mandatory"};
  }
  
  return request;
}

template <>
inline dto::UpdateRegistrationInterestRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateRegistrationInterestRequest request;
  
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateRegistrationInterestRequest"};
  }
  
  return request;
}

}  // namespace drogon

