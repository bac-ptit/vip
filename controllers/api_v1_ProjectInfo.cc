#include "api_v1_ProjectInfo.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> projectinfo::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProjectInfoRequest request) {
    auto response_dto{co_await service::project_info::Create(std::move(request))};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k201Created);
    resp->setBody(glz::write_json(response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> projectinfo::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string identifier, dto::UpdateProjectInfoRequest request) {
    co_await service::project_info::Update(std::move(identifier), std::move(request));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> projectinfo::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string identifier) {
    co_await service::project_info::Delete(std::move(identifier));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> projectinfo::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string identifier) {
  auto response_dto{co_await service::project_info::GetById(std::move(identifier))};
  if (response_dto) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(*response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
}

Task<> projectinfo::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
    auto list{co_await service::project_info::GetAll()};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(list).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
