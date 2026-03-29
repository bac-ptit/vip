//
// Created by bac on 3/27/26.
//

module;
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <glaze/glaze.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <trantor/utils/Date.h>
#include <vector>

export module dto:admin;

export namespace dto {

struct RegisterAdminRequest {
  std::string username;
  std::string password;
  std::string full_name;
};

struct LoginRequest {
  std::string username;
  std::string password;
};

struct UpdateProfileRequest {
  std::optional<std::string> username;
  std::optional<std::string> full_name;
};

struct ChangePasswordRequest {
  std::string old_password;
  std::string new_password;
};

struct AdminResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> username;
  std::shared_ptr<std::string> full_name;
  std::shared_ptr<bool> is_active;
  std::shared_ptr<trantor::Date> created_at;
  std::shared_ptr<trantor::Date> updated_at;
};

}  // namespace dto

// Drogon template specializations for automatic JSON conversion
namespace drogon {

template <>
inline dto::RegisterAdminRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::RegisterAdminRequest request;
  
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for RegisterAdminRequest"};
  }
  
  if (request.username.empty() || request.password.empty() || 
      request.full_name.empty()) {
    throw std::runtime_error{"Missing required fields"};
  }
  
  return request;
}

template <>
inline dto::LoginRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::LoginRequest request;
  
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for LoginRequest"};
  }
  
  if (request.username.empty() || request.password.empty()) {
    throw std::runtime_error{"Missing required fields"};
  }
  
  return request;
}

template <>
inline dto::UpdateProfileRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateProfileRequest request;
  
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateProfileRequest"};
  }
  
  return request;
}

template <>
inline dto::ChangePasswordRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::ChangePasswordRequest request;
  
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for ChangePasswordRequest"};
  }
  
  if (request.old_password.empty() || request.new_password.empty()) {
    throw std::runtime_error{"Missing required fields"};
  }
  
  return request;
}

}  // namespace drogon
