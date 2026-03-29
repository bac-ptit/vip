#include "api_v1_AdminSessions.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> adminsessions::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::admin_sessions::GetById(id)};
  if (response) {
    auto json{glz::write_json(*response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
    co_return;
  }
  callback(HttpResponse::newNotFoundResponse());
  co_return;
}

Task<> adminsessions::GetByAdminId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
  auto response{co_await service::admin_sessions::GetByAdminId(admin_id)};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("[]"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> adminsessions::Revoke(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::admin_sessions::RevokeSession(id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> adminsessions::RevokeAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
  co_await service::admin_sessions::RevokeAllAdminSessions(admin_id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}
