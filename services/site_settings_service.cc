//
// Created by bac on 3/28/26.
//
#include <drogon/drogon.h>
#include <models/SiteSettings.h>
#include <optional>
#include <string>
#include <vector>
#include "services/site_settings_service.h"
#include "repositories/repo.h"
#include "dto/dto.h"
#include "domains/domain.h"

using namespace drogon;

namespace {
dto::SiteSettingsResponse ToSiteSettingsResponse(const domain::SiteSettings& s) {
  dto::SiteSettingsResponse resp;
  resp.id = s.getId();
  resp.hotline = s.getHotline();
  resp.email = s.getEmail();
  resp.address = s.getAddress();
  resp.zalo_link = s.getZaloLink();
  resp.facebook_link = s.getFacebookLink();
  resp.logo_url = s.getLogoUrl();
  resp.tagline = s.getTagline();
  resp.updated_by = s.getUpdatedBy();
  resp.updated_at = s.getUpdatedAt();
  return resp;
}
}  // namespace

drogon::Task<std::optional<dto::SiteSettingsResponse>> service::site_settings::GetSettings() {
  auto s{co_await repo::site_settings::GetSettings()};
  if (s) {
    co_return ToSiteSettingsResponse(*s);
  }
  co_return std::nullopt;
}

drogon::Task<void> service::site_settings::Update(
    dto::UpdateSiteSettingsRequest request, std::optional<std::string> admin_id) {
  auto existing{co_await repo::site_settings::GetSettings()};
  
  domain::SiteSettings s;
  if (existing) {
    s = std::move(*existing);
  }

  if (request.hotline) s.setHotline(std::move(*request.hotline));
  if (request.email) s.setEmail(std::move(*request.email));
  if (request.address) s.setAddress(std::move(*request.address));
  if (request.zalo_link) s.setZaloLink(std::move(*request.zalo_link));
  if (request.facebook_link) s.setFacebookLink(std::move(*request.facebook_link));
  if (request.logo_url) s.setLogoUrl(std::move(*request.logo_url));
  if (request.tagline) s.setTagline(std::move(*request.tagline));
  if (admin_id) s.setUpdatedBy(*admin_id);

  if (existing) {
    co_await repo::site_settings::Update(s);
  } else {
    co_await repo::site_settings::Create(s);
  }
}
