#include "api_v1_AdminSessions.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> adminsessions::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string session_id) {
    auto response_dto{co_await service::admin_sessions::GetById(std::move(session_id))};
    if (!response_dto) {
        callback(HttpResponse::newNotFoundResponse());
        co_return;
    }
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(*response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> adminsessions::GetByAdminId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
    auto sessions{co_await service::admin_sessions::GetByAdminId(std::move(admin_id))};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(sessions).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> adminsessions::Revoke(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string session_id) {
    co_await service::admin_sessions::RevokeSession(std::move(session_id));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> adminsessions::RevokeAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
    co_await service::admin_sessions::RevokeAllAdminSessions(std::move(admin_id));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}
