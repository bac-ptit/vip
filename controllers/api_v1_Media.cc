#include "api_v1_Media.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> media::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateMediaRequest request) {
    auto response_dto{co_await service::media::Create(std::move(request), std::nullopt)};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k201Created);
    resp->setBody(glz::write_json(response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> media::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string media_id, dto::UpdateMediaRequest request) {
    co_await service::media::Update(std::move(media_id), std::move(request));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> media::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string media_id) {
    co_await service::media::Delete(std::move(media_id));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> media::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string media_id) {
  auto response_dto{co_await service::media::GetById(std::move(media_id))};
  if (response_dto) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(*response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
}

Task<> media::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
    auto list{co_await service::media::GetAll()};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(list).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> media::GetActive(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
    auto list{co_await service::media::GetActiveMedia()};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(list).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
