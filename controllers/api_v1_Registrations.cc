
#include "api_v1_Registrations.h"
#include <glaze/glaze.hpp>
import std;

using namespace api::v1;

Task<> registrations::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateRegistrationRequest request) {
  try {
    auto response{co_await service::registrations::Create(std::move(request))};
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

Task<> registrations::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateRegistrationRequest request) {
  try {
    co_await service::registrations::Update(std::move(id), std::move(request));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> registrations::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    co_await service::registrations::Delete(std::move(id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k404NotFound);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> registrations::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::registrations::GetById(std::move(id))};
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

Task<> registrations::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::registrations::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
