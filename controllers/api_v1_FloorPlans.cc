#include "api_v1_FloorPlans.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> floorplans::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateFloorPlanRequest request) {
    auto response_dto{co_await service::floor_plans::Create(std::move(request), std::nullopt)};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k201Created);
    resp->setBody(glz::write_json(response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> floorplans::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string floor_plan_id, dto::UpdateFloorPlanRequest request) {
    co_await service::floor_plans::Update(std::move(floor_plan_id), std::move(request));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> floorplans::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string floor_plan_id) {
    co_await service::floor_plans::Delete(std::move(floor_plan_id));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> floorplans::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string floor_plan_id) {
  auto response_dto{co_await service::floor_plans::GetById(std::move(floor_plan_id))};
  if (response_dto) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(*response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
}

Task<> floorplans::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
    auto list{co_await service::floor_plans::GetAll()};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(list).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> floorplans::GetActive(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
    auto list{co_await service::floor_plans::GetActive()};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(list).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}
