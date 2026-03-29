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

export module repo:activity_logs;

import domain;

using namespace drogon_model::qlattt;

export namespace repo::activity_logs {

[[nodiscard]] drogon::Task<std::optional<domain::ActivityLogs>> FindById(const std::string& id) {
  drogon::orm::CoroMapper<ActivityLogs> mapper{drogon::app().getDbClient()};
  try {
    auto res = co_await mapper.findByPrimaryKey(id);
    co_return domain::ActivityLogs{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::vector<domain::ActivityLogs>> FindByAdminId(const std::string& adminId) {
  drogon::orm::CoroMapper<ActivityLogs> mapper{drogon::app().getDbClient()};
  auto logs = co_await mapper.findBy(drogon::orm::Criteria(ActivityLogs::Cols::_admin_id, drogon::orm::CompareOperator::EQ, adminId));
  std::vector<domain::ActivityLogs> domain_logs;
  domain_logs.reserve(logs.size());
  for (auto& l : logs) {
    domain_logs.emplace_back(std::move(l));
  }
  co_return domain_logs;
}

[[nodiscard]] drogon::Task<std::vector<domain::ActivityLogs>> FindAll() {
  drogon::orm::CoroMapper<ActivityLogs> mapper{drogon::app().getDbClient()};
  auto logs = co_await mapper.findAll();
  std::vector<domain::ActivityLogs> domain_logs;
  domain_logs.reserve(logs.size());
  for (auto& l : logs) {
    domain_logs.emplace_back(std::move(l));
  }
  co_return domain_logs;
}

[[nodiscard]] drogon::Task<void> Create(const ActivityLogs& log) {
  drogon::orm::CoroMapper<ActivityLogs> mapper{drogon::app().getDbClient()};
  co_await mapper.insert(log);
}

}  // namespace repo::activity_logs
