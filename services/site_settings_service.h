#pragma once
#include <optional>
#include <string>
//
// Created by bac on 3/28/26.
//


#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>




#include <dto/dto.h>

namespace service::site_settings {

/**
 * @brief Get global site settings.
 */
[[nodiscard]] drogon::Task<std::optional<dto::SiteSettingsResponse>> GetSettings();

/**
 * @brief Update site settings.
 */
[[nodiscard]] drogon::Task<void> Update(
    dto::UpdateSiteSettingsRequest request, std::optional<std::string> admin_id);

}  // namespace service::site_settings
