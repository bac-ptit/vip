#include "activity_logs_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::activity_logs {

using ActivityLogsModel = drogon_model::qlattt::ActivityLogs;

drogon::Task<std::optional<domain::ActivityLogs>> FindById(std::string identifier) {
  drogon::orm::CoroMapper<ActivityLogsModel> mapper{drogon::app().getDbClient()};
  try {
    auto res{co_await mapper.findByPrimaryKey(identifier)};
    co_return domain::ActivityLogs{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::vector<domain::ActivityLogs>> FindByAdminId(std::string admin_id) {
  drogon::orm::CoroMapper<ActivityLogsModel> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(ActivityLogsModel::Cols::_admin_id, drogon::orm::CompareOperator::EQ, std::move(admin_id)))};
  
  co_return results | std::views::transform([](auto& r) {
    return domain::ActivityLogs{std::move(r)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<std::vector<domain::ActivityLogs>> FindAll() {
  drogon::orm::CoroMapper<ActivityLogsModel> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findAll()};
  
  co_return results | std::views::transform([](auto& r) {
    return domain::ActivityLogs{std::move(r)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<void> Create(domain::ActivityLogs log) {
  drogon::orm::CoroMapper<ActivityLogsModel> mapper{drogon::app().getDbClient()};
  co_await mapper.insert(log);
}

}  // namespace repo::activity_logs
