#include <utility>
#include <optional>
#include <string>
#include "site_settings_service.h"

#include <drogon/drogon.h>
#include <models/SiteSettings.h>

#include <repositories/repo.h>
#include <dto/dto.h>
#include <domains/domain.h>

using namespace drogon;

namespace {
dto::SiteSettingsResponse ToSiteSettingsResponse(const domain::SiteSettings& site_settings) {
  dto::SiteSettingsResponse response;
  response.id = site_settings.getId();
  response.hotline = site_settings.getHotline();
  response.email = site_settings.getEmail();
  response.address = site_settings.getAddress();
  response.zalo_link = site_settings.getZaloLink();
  response.facebook_link = site_settings.getFacebookLink();
  response.logo_url = site_settings.getLogoUrl();
  response.tagline = site_settings.getTagline();
  response.updated_by = site_settings.getUpdatedBy();
  response.updated_at = site_settings.getUpdatedAt();
  return response;
}
}  // namespace

drogon::Task<std::optional<dto::SiteSettingsResponse>> service::site_settings::GetSettings() {
  auto site_settings_opt{co_await repo::site_settings::GetSettings()};
  if (site_settings_opt) {
    co_return ToSiteSettingsResponse(*site_settings_opt);
  }
  co_return std::nullopt;
}

drogon::Task<void> service::site_settings::Update(
    dto::UpdateSiteSettingsRequest request, std::optional<std::string> admin_id) {
  auto existing_opt{co_await repo::site_settings::GetSettings()};
  
  domain::SiteSettings site_settings;
  if (existing_opt) {
    site_settings = std::move(*existing_opt);
  }

  if (request.hotline) site_settings.setHotline(std::move(*request.hotline));
  if (request.email) site_settings.setEmail(std::move(*request.email));
  if (request.address) site_settings.setAddress(std::move(*request.address));
  if (request.zalo_link) site_settings.setZaloLink(std::move(*request.zalo_link));
  if (request.facebook_link) site_settings.setFacebookLink(std::move(*request.facebook_link));
  if (request.logo_url) site_settings.setLogoUrl(std::move(*request.logo_url));
  if (request.tagline) site_settings.setTagline(std::move(*request.tagline));
  if (admin_id) site_settings.setUpdatedBy(*admin_id);

  if (existing_opt) {
    co_await repo::site_settings::Update(site_settings);
  } else {
    co_await repo::site_settings::Create(site_settings);
  }
}
