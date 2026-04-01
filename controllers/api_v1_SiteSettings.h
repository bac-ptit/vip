#pragma once
#include <functional>

#include <drogon/HttpController.h>

#include "dto/dto.h"
#include "services/service.h"

using namespace drogon;

namespace api::v1 {

class sitesettings : public drogon::HttpController<sitesettings> {
 public:
  METHOD_LIST_BEGIN
  
  METHOD_ADD(sitesettings::Get, "", drogon::Get);
  METHOD_ADD(sitesettings::Update, "", drogon::Put, "filter::AuthFilter");
  
  METHOD_LIST_END

  Task<> Get(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback);
  Task<> Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::UpdateSiteSettingsRequest request);
};

}  // namespace api::v1
