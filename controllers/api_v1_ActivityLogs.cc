
#include "api_v1_ActivityLogs.h"
#include <glaze/glaze.hpp>
import std;

using namespace api::v1;

Task<> activitylogs::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  try {
    auto response{co_await service::activity_logs::GetAll()};
    auto json{glz::write_json(response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> activitylogs::GetByAdminId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
  try {
    auto response{co_await service::activity_logs::GetByAdminId(std::move(admin_id))};
    auto json{glz::write_json(response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}
