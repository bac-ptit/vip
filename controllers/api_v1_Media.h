#pragma once

#include <functional>
#include <string>

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class media : public drogon::HttpController<media> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(media::Create, "", drogon::Post, "filter::AuthFilter");
  METHOD_ADD(media::Update, "/{1}", drogon::Put, "filter::AuthFilter");
  METHOD_ADD(media::Delete, "/{1}", drogon::Delete, "filter::AuthFilter");
  METHOD_ADD(media::GetById, "/{1}", drogon::Get);
  METHOD_ADD(media::GetAll, "", drogon::Get);
  METHOD_ADD(media::GetActive, "/active", drogon::Get);
  
  METHOD_LIST_END

  [[nodiscard]] Task<> Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateMediaRequest request);
  [[nodiscard]] Task<> Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateMediaRequest request);
  [[nodiscard]] Task<> Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  [[nodiscard]] Task<> GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  [[nodiscard]] Task<> GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
  [[nodiscard]] Task<> GetActive(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
};

}  // namespace api::v1
