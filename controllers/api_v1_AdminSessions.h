#pragma once

#include <functional>
#include <string>

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class adminsessions : public drogon::HttpController<adminsessions> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(adminsessions::GetById, "/{1}", drogon::Get, "filter::AuthFilter");
  METHOD_ADD(adminsessions::GetByAdminId, "/admin/{1}", drogon::Get, "filter::AuthFilter");
  METHOD_ADD(adminsessions::Revoke, "/{1}/revoke", drogon::Post, "filter::AuthFilter");
  METHOD_ADD(adminsessions::RevokeAll, "/admin/{1}/revoke-all", drogon::Post, "filter::AuthFilter");
  
  METHOD_LIST_END

  Task<> GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetByAdminId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id);
  Task<> Revoke(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> RevokeAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id);
};

}  // namespace api::v1
