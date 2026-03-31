#pragma once
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::site_settings {

[[nodiscard]] inline drogon::Task<std::optional<domain::SiteSettings>> GetSettings() {
  drogon::orm::CoroMapper<drogon_model::qlattt::SiteSettings> mapper{
      drogon::app().getDbClient()};
  auto results{co_await mapper.findAll()};
  if (results.empty()) {
    co_return std::nullopt;
  }
  co_return domain::SiteSettings{std::move(results[0])};
}

[[nodiscard]] inline drogon::Task<void> Update(domain::SiteSettings s) {
  drogon::orm::CoroMapper<drogon_model::qlattt::SiteSettings> mapper{
      drogon::app().getDbClient()};
  co_await mapper.update(s);
}

[[nodiscard]] inline drogon::Task<void> Create(domain::SiteSettings s) {
  drogon::orm::CoroMapper<drogon_model::qlattt::SiteSettings> mapper{
      drogon::app().getDbClient()};
  co_await mapper.insert(s);
}

}  // namespace repo::site_settings
