#include "api_v1_Admin.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> admin::Register(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::RegisterAdminRequest&& request) {
  auto response{co_await service::admin::Register(std::move(request))};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> admin::Login(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::LoginRequest&& request) {
  auto response{co_await service::admin::Login(std::move(request))};
  if (response) {
    // 1. Lưu vào Session của Drogon (Dùng cho Cookie-based auth)
    if (auto session{req->session()}) {
      session->insert("admin_id", response->id);
    }
    
    auto json{glz::write_json(*response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    
    // 2. Trả về admin_id qua Header (Dùng cho Header-based auth)
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
  co_return;
}

Task<> admin::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::admin::GetById(id)};
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

Task<> admin::GetByUsername(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string username) {
  auto response{co_await service::admin::GetByUsername(username)};
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

Task<> admin::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::admin::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> admin::GetActiveAdmins(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::admin::GetActiveAdmins()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> admin::UpdateProfile(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProfileRequest&& request) {
  co_await service::admin::UpdateProfile(id, std::move(request));
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> admin::ChangePassword(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::ChangePasswordRequest&& request) {
  co_await service::admin::ChangePassword(id, std::move(request));
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> admin::Deactivate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::admin::Deactivate(id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> admin::Activate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::admin::Activate(id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> admin::CheckUsername(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string username) {
  bool available{co_await service::admin::IsUsernameAvailable(username)};
  auto json{glz::write_json(std::map<std::string, bool>{{"available", available}})};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
