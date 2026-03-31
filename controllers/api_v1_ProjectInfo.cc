
#include "api_v1_ProjectInfo.h"
#include <glaze/glaze.hpp>
import std;

using namespace api::v1;

Task<> projectinfo::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProjectInfoRequest request) {
  try {
    auto response{co_await service::project_info::Create(std::move(request))};
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

Task<> projectinfo::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProjectInfoRequest request) {
  try {
    co_await service::project_info::Update(std::move(id), std::move(request));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> projectinfo::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    co_await service::project_info::Delete(std::move(id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k404NotFound);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> projectinfo::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::project_info::GetById(std::move(id))};
  if (response) {
    auto json{glz::write_json(*response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
  co_return;
}

Task<> projectinfo::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::project_info::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
