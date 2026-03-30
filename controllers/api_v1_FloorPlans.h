#pragma once

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class floorplans : public drogon::HttpController<floorplans> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(floorplans::Create, "", drogon::Post, "filter::AuthFilter");
  METHOD_ADD(floorplans::Update, "/{1}", drogon::Put, "filter::AuthFilter");
  METHOD_ADD(floorplans::Delete, "/{1}", drogon::Delete, "filter::AuthFilter");
  METHOD_ADD(floorplans::GetById, "/{1}", drogon::Get);
  METHOD_ADD(floorplans::GetAll, "", drogon::Get);
  METHOD_ADD(floorplans::GetActive, "/active", drogon::Get);
  
  METHOD_LIST_END

  Task<> Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateFloorPlanRequest request);
  Task<> Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateFloorPlanRequest request);
  Task<> Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
  Task<> GetActive(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
};

}  // namespace api::v1
