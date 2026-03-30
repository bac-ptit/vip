#include "api_v1_Media.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> media::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateMediaRequest request) {
  try {
    std::optional<std::string> admin_id;
    if (auto session{req->session()}) {
      admin_id = session->getOptional<std::string>("admin_id");
    }
    
    auto response{co_await service::media::Create(std::move(request), std::move(admin_id))};
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

Task<> media::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateMediaRequest request) {
  try {
    co_await service::media::Update(std::move(id), std::move(request));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> media::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    co_await service::media::Delete(std::move(id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> media::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    auto response{co_await service::media::GetById(std::move(id))};
    if (response) {
      auto json{glz::write_json(*response)};
      auto resp{HttpResponse::newHttpResponse()};
      resp->setBody(std::move(json).value_or("{}"));
      resp->setContentTypeCode(CT_APPLICATION_JSON);
      callback(resp);
      co_return;
    }
    callback(HttpResponse::newNotFoundResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> media::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  try {
    auto response{co_await service::media::GetAll()};
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

Task<> media::GetActive(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  try {
    auto response{co_await service::media::GetActiveMedia()};
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
