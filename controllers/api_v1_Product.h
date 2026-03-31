#pragma once

#include <drogon/HttpController.h>

#include "dto/dto.h"
#include "services/service.h"

using namespace drogon;

namespace api::v1 {

class products : public drogon::HttpController<products> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(products::Create, "", drogon::Post, "filter::AuthFilter");
  METHOD_ADD(products::Update, "/{1}", drogon::Put, "filter::AuthFilter");
  METHOD_ADD(products::Delete, "/{1}", drogon::Delete, "filter::AuthFilter");
  METHOD_ADD(products::GetById, "/{1}", drogon::Get);
  METHOD_ADD(products::GetAll, "", drogon::Get);
  METHOD_ADD(products::Search, "/search", drogon::Get);
  
  METHOD_LIST_END

  Task<> Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProductRequest request);
  Task<> Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProductRequest request);
  Task<> Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
  Task<> Search(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
};

}  // namespace api::v1
