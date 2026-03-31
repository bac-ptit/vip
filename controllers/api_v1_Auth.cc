
#include "api_v1_Auth.h"
#include <glaze/glaze.hpp>
import std;

using namespace api::v1;

Task<> auth::Register(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::RegisterAdminRequest request) {
  try {
    auto response{co_await service::admin::Register(std::move(request))};
    auto json{glz::write_json(response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
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

Task<> auth::Login(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::LoginRequest request) {
  try {
    auto response{co_await service::admin::Login(std::move(request))};
    if (response) {
      if (auto session{req->session()}) {
        session->insert("admin_id", *response->id);
      }
      
      auto json{glz::write_json(*response)};
      auto resp{HttpResponse::newHttpResponse()};
      resp->setBody(std::move(json).value_or("{}"));
      resp->setContentTypeCode(CT_APPLICATION_JSON);
      resp->addHeader("X-Admin-Id", *response->id);
      
      callback(resp);
      co_return;
    }

    auto json{glz::write_json(std::map<std::string, std::string>{{"error", "Invalid credentials"}})};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    resp->setStatusCode(k401Unauthorized);
    callback(resp);
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k403Forbidden);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> auth::Logout(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  if (auto session{req->session()}) {
    session->clear();
  }
  auto resp{HttpResponse::newHttpResponse()};
  auto json{glz::write_json(std::map<std::string, std::string>{{"message", "Logged out successfully"}})};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
