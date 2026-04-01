#include "api_v1_Admin.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> admin::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
  auto response_dto{co_await service::admin::GetById(std::move(admin_id))};
  if (response_dto) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(*response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
}

Task<> admin::GetByUsername(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string username) {
  auto response_dto{co_await service::admin::GetByUsername(std::move(username))};
  if (response_dto) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(*response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
}

Task<> admin::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto list{co_await service::admin::GetAll()};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(glz::write_json(list).value_or("[]"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
}

Task<> admin::GetActiveAdmins(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto list{co_await service::admin::GetActiveAdmins()};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(glz::write_json(list).value_or("[]"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
}

Task<> admin::UpdateProfile(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id, dto::UpdateProfileRequest request) {
    co_await service::admin::UpdateProfile(std::move(admin_id), std::move(request));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> admin::ChangePassword(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id, dto::ChangePasswordRequest request) {
    co_await service::admin::ChangePassword(std::move(admin_id), std::move(request));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> admin::Deactivate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
    co_await service::admin::Deactivate(std::move(admin_id));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> admin::Activate(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string admin_id) {
    co_await service::admin::Activate(std::move(admin_id));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> admin::CheckUsername(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string username) {
    auto available{co_await service::admin::IsUsernameAvailable(std::move(username))};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(available ? "true" : "false");
    callback(resp);
}
