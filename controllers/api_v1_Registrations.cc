#include "api_v1_Registrations.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> registrations::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateRegistrationRequest request) {
    auto response_dto{co_await service::registrations::Create(std::move(request))};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k201Created);
    resp->setBody(glz::write_json(response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> registrations::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string registration_id, dto::UpdateRegistrationRequest request) {
    co_await service::registrations::Update(std::move(registration_id), std::move(request));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> registrations::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string registration_id) {
    co_await service::registrations::Delete(std::move(registration_id));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> registrations::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string registration_id) {
  auto response_dto{co_await service::registrations::GetById(std::move(registration_id))};
  if (response_dto) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(*response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
}

Task<> registrations::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
    auto list{co_await service::registrations::GetAll()};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(list).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
