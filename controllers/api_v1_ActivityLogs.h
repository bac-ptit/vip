#pragma once
#include <string>

#include <functional>
#include <string>

#include <drogon/HttpController.h>

#include <dto/dto.h>
#include <services/service.h>

using namespace drogon;

namespace api::v1 {

class activitylogs : public drogon::HttpController<activitylogs> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(activitylogs::GetAll, "", drogon::Get, "filter::AuthFilter");
  METHOD_ADD(activitylogs::GetByAdminId, "/admin/{1}", drogon::Get, "filter::AuthFilter");
  
  METHOD_LIST_END

  Task<> GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
  Task<> GetByAdminId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id);
};

}  // namespace api::v1
