#pragma once
#include <models/SiteSettings.h>

namespace domain {

class SiteSettings : public drogon_model::qlattt::SiteSettings {
 public:
  using drogon_model::qlattt::SiteSettings::SiteSettings;

  SiteSettings(drogon_model::qlattt::SiteSettings&& base) noexcept
      : drogon_model::qlattt::SiteSettings(std::move(base)) {}
};

}  // namespace domain
