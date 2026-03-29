#include "api_v1_ActivityLogs.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> activitylogs::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::activity_logs::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("[]"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> activitylogs::GetByAdminId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
  auto response{co_await service::activity_logs::GetByAdminId(admin_id)};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("[]"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
