#pragma once
#include <drogon/orm/CoroMapper.h>
#include <drogon/drogon.h>
#include <model.h>
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::admin_sessions {

[[nodiscard]] inline drogon::Task<std::optional<domain::AdminSessions>> FindById(std::string id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  try {
    auto result{co_await mapper.findByPrimaryKey(id)};
    co_return domain::AdminSessions{std::move(result)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] inline drogon::Task<std::optional<domain::AdminSessions>> FindByRefreshToken(std::string token) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(
      drogon_model::qlattt::AdminSessions::Cols::_refresh_token,
      drogon::orm::CompareOperator::EQ, token))};
  if (results.empty()) {
    co_return std::nullopt;
  }
  co_return domain::AdminSessions{std::move(results[0])};
}

[[nodiscard]] inline drogon::Task<std::vector<domain::AdminSessions>> FindByAdminId(std::string adminId) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(
      drogon_model::qlattt::AdminSessions::Cols::_admin_id,
      drogon::orm::CompareOperator::EQ, adminId))};

  co_return results | std::views::transform([](auto&& r) {
    return domain::AdminSessions{std::move(r)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<void> Create(domain::AdminSessions session) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  co_await mapper.insert(session);
}

[[nodiscard]] inline drogon::Task<void> Update(domain::AdminSessions session) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  co_await mapper.update(session);
}

[[nodiscard]] inline drogon::Task<void> DeleteById(std::string id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  co_await mapper.deleteByPrimaryKey(id);
}

[[nodiscard]] inline drogon::Task<void> DeleteByAdminId(std::string adminId) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  co_await mapper.deleteBy(drogon::orm::Criteria(
      drogon_model::qlattt::AdminSessions::Cols::_admin_id,
      drogon::orm::CompareOperator::EQ, std::move(adminId)));
}

}  // namespace repo::admin_sessions
