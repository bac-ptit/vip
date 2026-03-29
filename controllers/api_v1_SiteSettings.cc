#include "api_v1_SiteSettings.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> sitesettings::Get(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::site_settings::GetSettings()};
  if (response) {
    auto json{glz::write_json(*response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
    co_return;
  }
  callback(HttpResponse::newNotFoundResponse());
  co_return;
}

Task<> sitesettings::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::UpdateSiteSettingsRequest&& request) {
  std::optional<std::string> admin_id;
  if (auto session{req->session()}) {
    admin_id = session->getOptional<std::string>("admin_id");
  }
  co_await service::site_settings::Update(std::move(request), admin_id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}
