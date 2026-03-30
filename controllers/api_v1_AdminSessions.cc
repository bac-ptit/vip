#include "api_v1_AdminSessions.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> adminsessions::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    auto response{co_await service::admin_sessions::GetById(std::move(id))};
    if (response) {
      auto json{glz::write_json(*response)};
      auto resp{HttpResponse::newHttpResponse()};
      resp->setBody(std::move(json).value_or("{}"));
      resp->setContentTypeCode(CT_APPLICATION_JSON);
      callback(resp);
    } else {
      callback(HttpResponse::newNotFoundResponse());
    }
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> adminsessions::GetByAdminId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
  try {
    auto response{co_await service::admin_sessions::GetByAdminId(std::move(admin_id))};
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

Task<> adminsessions::Revoke(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    co_await service::admin_sessions::RevokeSession(std::move(id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> adminsessions::RevokeAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
  try {
    co_await service::admin_sessions::RevokeAllAdminSessions(std::move(admin_id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}
