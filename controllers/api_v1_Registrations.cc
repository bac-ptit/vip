#include "api_v1_Registrations.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> registrations::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateRegistrationRequest&& request) {
  auto response{co_await service::registrations::Create(std::move(request))};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> registrations::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateRegistrationRequest&& request) {
  co_await service::registrations::Update(id, std::move(request));
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> registrations::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::registrations::Delete(id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> registrations::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::registrations::GetById(id)};
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

Task<> registrations::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::registrations::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
