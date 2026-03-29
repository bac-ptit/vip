//
// Created by bac on 3/28/26.
//

module;
#include <models/SiteSettings.h>

export module domain:site_settings;

export namespace domain {

class SiteSettings : public drogon_model::qlattt::SiteSettings {
 public:
  using drogon_model::qlattt::SiteSettings::SiteSettings;

  SiteSettings(drogon_model::qlattt::SiteSettings&& base) noexcept
      : drogon_model::qlattt::SiteSettings(std::move(base)) {}
};

}  // namespace domain
