//
// Created by bac on 3/29/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <string>
#include <vector>
#include <ranges>

export module repo:activity_logs;

import domain;

using namespace drogon_model::qlattt;

export namespace repo::activity_logs {

[[nodiscard]] drogon::Task<std::optional<domain::ActivityLogs>> FindById(std::string id) {
  drogon::orm::CoroMapper<ActivityLogs> mapper{drogon::app().getDbClient()};
  try {
    auto res = co_await mapper.findByPrimaryKey(id);
    co_return domain::ActivityLogs{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::vector<domain::ActivityLogs>> FindByAdminId(std::string adminId) {
  drogon::orm::CoroMapper<ActivityLogs> mapper{drogon::app().getDbClient()};
  auto logs = co_await mapper.findBy(drogon::orm::Criteria(ActivityLogs::Cols::_admin_id, drogon::orm::CompareOperator::EQ, adminId));
  
  co_return logs | std::views::transform([](auto&& l) {
    return domain::ActivityLogs{std::move(l)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] drogon::Task<std::vector<domain::ActivityLogs>> FindAll() {
  drogon::orm::CoroMapper<ActivityLogs> mapper{drogon::app().getDbClient()};
  auto logs = co_await mapper.findAll();
  
  co_return logs | std::views::transform([](auto&& l) {
    return domain::ActivityLogs{std::move(l)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] drogon::Task<void> Create(ActivityLogs log) {
  drogon::orm::CoroMapper<ActivityLogs> mapper{drogon::app().getDbClient()};
  co_await mapper.insert(log);
}

}  // namespace repo::activity_logs
