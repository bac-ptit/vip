#include "api_v1_RegistrationInterests.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> registrationinterests::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateRegistrationInterestRequest&& request) {
  auto response{co_await service::registration_interests::Create(std::move(request))};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> registrationinterests::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::registration_interests::Delete(id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> registrationinterests::GetByRegistrationId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string registration_id) {
  auto response{co_await service::registration_interests::GetByRegistrationId(registration_id)};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
