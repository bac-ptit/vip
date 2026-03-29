#pragma once

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class activitylogs : public drogon::HttpController<activitylogs> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(activitylogs::GetAll, "", drogon::Get);
  METHOD_ADD(activitylogs::GetByAdminId, "/admin/{1}", drogon::Get);
  
  METHOD_LIST_END

  Task<> GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
  Task<> GetByAdminId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id);
};

}  // namespace api::v1
