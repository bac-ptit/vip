#include "site_settings_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>

namespace repo::site_settings {

using SiteSettingsModel = drogon_model::qlattt::SiteSettings;

drogon::Task<std::optional<domain::SiteSettings>> GetSettings() {
  drogon::orm::CoroMapper<SiteSettingsModel> mapper{drogon::app().getFastDbClient()};
  auto results{co_await mapper.findAll()};
  if (results.empty()) {
    co_return std::nullopt;
  }
  co_return domain::SiteSettings{std::move(results[0])};
}

drogon::Task<void> Update(domain::SiteSettings s) {
  drogon::orm::CoroMapper<SiteSettingsModel> mapper{drogon::app().getFastDbClient()};
  co_await mapper.update(s);
}

drogon::Task<void> Create(domain::SiteSettings s) {
  drogon::orm::CoroMapper<SiteSettingsModel> mapper{drogon::app().getFastDbClient()};
  co_await mapper.insert(s);
}

}  // namespace repo::site_settings
