
#include "api_v1_Admin.h"
#include <glaze/glaze.hpp>
import std;

using namespace api::v1;

Task<> admin::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::admin::GetById(std::move(id))};
  if (response) {
    auto json{glz::write_json(*response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
  co_return;
}

Task<> admin::GetByUsername(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string username) {
  auto response{co_await service::admin::GetByUsername(std::move(username))};
  if (response) {
    auto json{glz::write_json(*response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
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

Task<> admin::UpdateProfile(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProfileRequest request) {
  try {
    co_await service::admin::UpdateProfile(std::move(id), std::move(request));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> admin::ChangePassword(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::ChangePasswordRequest request) {
  try {
    co_await service::admin::ChangePassword(std::move(id), std::move(request));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> admin::Deactivate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    co_await service::admin::Deactivate(std::move(id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k404NotFound);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> admin::Activate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    co_await service::admin::Activate(std::move(id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k404NotFound);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> admin::CheckUsername(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string username) {
  auto available{co_await service::admin::IsUsernameAvailable(std::move(username))};
  auto json{glz::write_json(std::map<std::string, bool>{{"available", available}})};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
