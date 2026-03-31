#pragma once
#include <drogon/HttpRequest.h>
#include <glaze/glaze.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <trantor/utils/Date.h>

namespace dto {

struct UpdateSiteSettingsRequest {
  std::optional<std::string> hotline;
  std::optional<std::string> email;
  std::optional<std::string> address;
  std::optional<std::string> zalo_link;
  std::optional<std::string> facebook_link;
  std::optional<std::string> logo_url;
  std::optional<std::string> tagline;
};

struct SiteSettingsResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> hotline;
  std::shared_ptr<std::string> email;
  std::shared_ptr<std::string> address;
  std::shared_ptr<std::string> zalo_link;
  std::shared_ptr<std::string> facebook_link;
  std::shared_ptr<std::string> logo_url;
  std::shared_ptr<std::string> tagline;
  std::shared_ptr<std::string> updated_by;
  std::shared_ptr<trantor::Date> updated_at;
};

}  // namespace dto

namespace drogon {

template <>
inline dto::UpdateSiteSettingsRequest fromRequest(const HttpRequest& req) {
  auto json_str{req.body()};
  dto::UpdateSiteSettingsRequest request;
  auto ec{glz::read_json(request, json_str)};
  if (ec) {
    throw std::runtime_error{"Invalid JSON for UpdateSiteSettingsRequest"};
  }
  return request;
}

}  // namespace drogon
