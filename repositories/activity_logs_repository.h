#pragma once
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::activity_logs {

[[nodiscard]] inline drogon::Task<std::optional<domain::ActivityLogs>> FindById(std::string id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::ActivityLogs> mapper{drogon::app().getDbClient()};
  try {
    auto res = co_await mapper.findByPrimaryKey(id);
    co_return domain::ActivityLogs{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] inline drogon::Task<std::vector<domain::ActivityLogs>> FindByAdminId(std::string adminId) {
  drogon::orm::CoroMapper<drogon_model::qlattt::ActivityLogs> mapper{drogon::app().getDbClient()};
  auto logs = co_await mapper.findBy(drogon::orm::Criteria(
      drogon_model::qlattt::ActivityLogs::Cols::_admin_id,
      drogon::orm::CompareOperator::EQ, adminId));

  co_return logs | std::views::transform([](auto&& l) {
    return domain::ActivityLogs{std::move(l)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<std::vector<domain::ActivityLogs>> FindAll() {
  drogon::orm::CoroMapper<drogon_model::qlattt::ActivityLogs> mapper{drogon::app().getDbClient()};
  auto logs = co_await mapper.findAll();

  co_return logs | std::views::transform([](auto&& l) {
    return domain::ActivityLogs{std::move(l)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<void> Create(drogon_model::qlattt::ActivityLogs log) {
  drogon::orm::CoroMapper<drogon_model::qlattt::ActivityLogs> mapper{drogon::app().getDbClient()};
  co_await mapper.insert(log);
}

}  // namespace repo::activity_logs
