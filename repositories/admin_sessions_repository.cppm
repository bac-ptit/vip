//
// Created by bac on 3/29/26.
//

module;
#include <drogon/orm/CoroMapper.h>
#include <drogon/drogon.h>
#include <model.h>
#include <optional>
#include <string>
#include <vector>

export module repo:admin_sessions;

import domain;

export namespace repo::admin_sessions {

[[nodiscard]] drogon::Task<std::optional<domain::AdminSessions>> FindById(const std::string& id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  try {
    auto result{co_await mapper.findByPrimaryKey(id)};
    co_return domain::AdminSessions{std::move(result)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::optional<domain::AdminSessions>> FindByRefreshToken(const std::string& token) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(drogon_model::qlattt::AdminSessions::Cols::_refresh_token, drogon::orm::CompareOperator::EQ, token))};
  if (results.empty()) {
    co_return std::nullopt;
  }
  co_return domain::AdminSessions{std::move(results[0])};
}

[[nodiscard]] drogon::Task<std::vector<domain::AdminSessions>> FindByAdminId(const std::string& adminId) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(drogon_model::qlattt::AdminSessions::Cols::_admin_id, drogon::orm::CompareOperator::EQ, adminId))};
  std::vector<domain::AdminSessions> domain_results;
  domain_results.reserve(results.size());
  for (auto& r : results) {
    domain_results.emplace_back(std::move(r));
  }
  co_return domain_results;
}

[[nodiscard]] drogon::Task<void> Create(const domain::AdminSessions& session) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  co_await mapper.insert(session);
}

[[nodiscard]] drogon::Task<void> Update(const domain::AdminSessions& session) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  co_await mapper.update(session);
}

[[nodiscard]] drogon::Task<void> DeleteById(const std::string& id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  co_await mapper.deleteByPrimaryKey(id);
}

[[nodiscard]] drogon::Task<void> DeleteByAdminId(const std::string& adminId) {
  drogon::orm::CoroMapper<drogon_model::qlattt::AdminSessions> mapper{drogon::app().getDbClient()};
  co_await mapper.deleteBy(drogon::orm::Criteria(drogon_model::qlattt::AdminSessions::Cols::_admin_id, drogon::orm::CompareOperator::EQ, adminId));
}

}  // namespace repo::admin_sessions
