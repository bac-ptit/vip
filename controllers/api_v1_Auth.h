#pragma once

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class auth : public drogon::HttpController<auth> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(auth::Register, "/register", drogon::Post);
  METHOD_ADD(auth::Login, "/login", drogon::Post);
  METHOD_ADD(auth::Logout, "/logout", drogon::Post, "filter::AuthFilter");
  
  METHOD_LIST_END

  Task<> Register(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::RegisterAdminRequest request);
  Task<> Login(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::LoginRequest request);
  Task<> Logout(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
};

}  // namespace api::v1
