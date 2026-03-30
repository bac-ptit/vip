#pragma once

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class projectinfo : public drogon::HttpController<projectinfo> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(projectinfo::Create, "", drogon::Post, "filter::AuthFilter");
  METHOD_ADD(projectinfo::Update, "/{1}", drogon::Put, "filter::AuthFilter");
  METHOD_ADD(projectinfo::Delete, "/{1}", drogon::Delete, "filter::AuthFilter");
  METHOD_ADD(projectinfo::GetById, "/{1}", drogon::Get);
  METHOD_ADD(projectinfo::GetAll, "", drogon::Get);
  
  METHOD_LIST_END

  Task<> Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProjectInfoRequest request);
  Task<> Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProjectInfoRequest request);
  Task<> Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
};

}  // namespace api::v1
