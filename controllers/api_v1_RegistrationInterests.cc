
#include "api_v1_RegistrationInterests.h"
#include <glaze/glaze.hpp>
import std;

using namespace api::v1;

Task<> registrationinterests::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateRegistrationInterestRequest request) {
  try {
    auto response{co_await service::registration_interests::Create(std::move(request))};
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

Task<> registrationinterests::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    co_await service::registration_interests::Delete(std::move(id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k404NotFound);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> registrationinterests::GetByRegistrationId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string registration_id) {
  try {
    auto response{co_await service::registration_interests::GetByRegistrationId(std::move(registration_id))};
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
