#include "api_v1_Media.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> media::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateMediaRequest&& request) {
  auto response{co_await service::media::Create(std::move(request), std::optional<std::string>{})};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> media::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateMediaRequest&& request) {
  co_await service::media::Update(id, std::move(request));
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> media::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::media::Delete(id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> media::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::media::GetById(id)};
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

Task<> media::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::media::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> media::GetActive(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::media::GetActiveMedia()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
