#include "api_v1_RegistrationInterests.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> registrationinterests::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateRegistrationInterestRequest request) {
    auto response_dto{co_await service::registration_interests::Create(std::move(request))};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k201Created);
    resp->setBody(glz::write_json(response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> registrationinterests::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string registration_interest_id) {
    co_await service::registration_interests::Delete(std::move(registration_interest_id));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> registrationinterests::GetByRegistrationId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string registration_id) {
    auto list{co_await service::registration_interests::GetByRegistrationId(std::move(registration_id))};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(list).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
