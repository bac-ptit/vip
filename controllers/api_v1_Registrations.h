#pragma once

#include <drogon/HttpController.h>

#include "dto/dto.h"
#include "services/service.h"

using namespace drogon;

namespace api::v1 {

class registrations : public drogon::HttpController<registrations> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(registrations::Create, "", drogon::Post);
  METHOD_ADD(registrations::Update, "/{1}", drogon::Put, "filter::AuthFilter");
  METHOD_ADD(registrations::Delete, "/{1}", drogon::Delete, "filter::AuthFilter");
  METHOD_ADD(registrations::GetById, "/{1}", drogon::Get, "filter::AuthFilter");
  METHOD_ADD(registrations::GetAll, "", drogon::Get, "filter::AuthFilter");
  
  METHOD_LIST_END

  Task<> Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateRegistrationRequest request);
  Task<> Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateRegistrationRequest request);
  Task<> Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
};

}  // namespace api::v1
