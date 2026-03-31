#pragma once
#include <drogon/drogon.h>
#include <optional>
#include <string>
#include "dto/dto.h"

namespace service::site_settings {

[[nodiscard]] drogon::Task<std::optional<dto::SiteSettingsResponse>> GetSettings();

[[nodiscard]] drogon::Task<void> Update(
    dto::UpdateSiteSettingsRequest request, std::optional<std::string> admin_id);

}  // namespace service::site_settings
