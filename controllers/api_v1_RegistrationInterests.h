#pragma once

#include <functional>
#include <string>

#include <drogon/HttpController.h>

import dto;
import service;

using namespace drogon;

namespace api::v1 {

class registrationinterests : public drogon::HttpController<registrationinterests> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(registrationinterests::Create, "", drogon::Post);
  METHOD_ADD(registrationinterests::Delete, "/{1}", drogon::Delete, "filter::AuthFilter");
  METHOD_ADD(registrationinterests::GetByRegistrationId, "/registration/{1}", drogon::Get, "filter::AuthFilter");
  
  METHOD_LIST_END

  Task<> Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateRegistrationInterestRequest request);
  Task<> Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id);
  Task<> GetByRegistrationId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string registration_id);
};

}  // namespace api::v1
