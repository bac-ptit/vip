#pragma once
#include <optional>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::site_settings {

[[nodiscard]] drogon::Task<std::optional<domain::SiteSettings>> GetSettings();
[[nodiscard]] drogon::Task<void> Update(domain::SiteSettings s);
[[nodiscard]] drogon::Task<void> Create(domain::SiteSettings s);

}  // namespace repo::site_settings
