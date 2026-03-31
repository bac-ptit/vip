
#include "api_v1_FloorPlans.h"
#include <glaze/glaze.hpp>
import std;

using namespace api::v1;

Task<> floorplans::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateFloorPlanRequest request) {
  try {
    std::optional<std::string> admin_id;
    if (auto session{req->session()}) {
      admin_id = session->getOptional<std::string>("admin_id");
    }
    auto response{co_await service::floor_plans::Create(std::move(request), std::move(admin_id))};
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

Task<> floorplans::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateFloorPlanRequest request) {
  try {
    co_await service::floor_plans::Update(std::move(id), std::move(request));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> floorplans::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    co_await service::floor_plans::Delete(std::move(id));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k404NotFound);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> floorplans::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::floor_plans::GetById(std::move(id))};
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

Task<> floorplans::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::floor_plans::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> floorplans::GetActive(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::floor_plans::GetActive()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
