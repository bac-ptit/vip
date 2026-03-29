#pragma once

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class productimages : public drogon::HttpController<productimages> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(productimages::Create, "", drogon::Post);
  METHOD_ADD(productimages::Update, "/{1}", drogon::Put);
  METHOD_ADD(productimages::Delete, "/{1}", drogon::Delete);
  METHOD_ADD(productimages::GetById, "/{1}", drogon::Get);
  METHOD_ADD(productimages::GetByProductId, "/product/{1}", drogon::Get);
  METHOD_ADD(productimages::GetActiveByProductId, "/product/{1}/active", drogon::Get);
  
  METHOD_LIST_END

  Task<> Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProductImageRequest&& request);
  Task<> Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProductImageRequest&& request);
  Task<> Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetByProductId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string product_id);
  Task<> GetActiveByProductId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string product_id);
};

}  // namespace api::v1
