#pragma once

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class admin : public drogon::HttpController<admin> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(admin::Register, "/register", drogon::Post);
  METHOD_ADD(admin::Login, "/login", drogon::Post);
  METHOD_ADD(admin::GetById, "/{1}", drogon::Get);
  METHOD_ADD(admin::GetByUsername, "/username/{1}", drogon::Get);
  METHOD_ADD(admin::GetAll, "", drogon::Get);
  METHOD_ADD(admin::GetActiveAdmins, "/active", drogon::Get);
  METHOD_ADD(admin::UpdateProfile, "/{1}/profile", drogon::Put);
  METHOD_ADD(admin::ChangePassword, "/{1}/password", drogon::Put);
  METHOD_ADD(admin::Deactivate, "/{1}/deactivate", drogon::Post);
  METHOD_ADD(admin::Activate, "/{1}/activate", drogon::Post);
  METHOD_ADD(admin::CheckUsername, "/check-username/{1}", drogon::Get);
  
  METHOD_LIST_END

  Task<> Register(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::RegisterAdminRequest&& request);
  Task<> Login(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::LoginRequest&& request);
  Task<> GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetByUsername(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string username);
  Task<> GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
  Task<> GetActiveAdmins(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
  Task<> UpdateProfile(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProfileRequest&& request);
  Task<> ChangePassword(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::ChangePasswordRequest&& request);
  Task<> Deactivate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> Activate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> CheckUsername(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string username);
};

}  // namespace api::v1
