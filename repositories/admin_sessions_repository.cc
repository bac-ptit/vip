#include "admin_sessions_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::admin_sessions {

using AdminSessionsModel = drogon_model::qlattt::AdminSessions;

drogon::Task<std::optional<domain::AdminSessions>> FindById(std::string session_id) {
  drogon::orm::CoroMapper<AdminSessionsModel> mapper{drogon::app().getFastDbClient()};
  try {
    auto result{co_await mapper.findByPrimaryKey(session_id)};
    co_return domain::AdminSessions{std::move(result)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::optional<domain::AdminSessions>> FindByRefreshToken(std::string token) {
  drogon::orm::CoroMapper<AdminSessionsModel> mapper{drogon::app().getFastDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(AdminSessionsModel::Cols::_refresh_token, drogon::orm::CompareOperator::EQ, token))};
  if (results.empty()) {
    co_return std::nullopt;
  }
  co_return domain::AdminSessions{std::move(results[0])};
}

drogon::Task<std::vector<domain::AdminSessions>> FindByAdminId(std::string admin_id) {
  drogon::orm::CoroMapper<AdminSessionsModel> mapper{drogon::app().getFastDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(AdminSessionsModel::Cols::_admin_id, drogon::orm::CompareOperator::EQ, admin_id))};
  
  co_return results | std::views::transform([](auto& r) {
    return domain::AdminSessions{std::move(r)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<void> Create(domain::AdminSessions session) {
  drogon::orm::CoroMapper<AdminSessionsModel> mapper{drogon::app().getFastDbClient()};
  co_await mapper.insert(session);
}

drogon::Task<void> Update(domain::AdminSessions session) {
  drogon::orm::CoroMapper<AdminSessionsModel> mapper{drogon::app().getFastDbClient()};
  co_await mapper.update(session);
}

drogon::Task<void> DeleteById(std::string session_id) {
  drogon::orm::CoroMapper<AdminSessionsModel> mapper{drogon::app().getFastDbClient()};
  co_await mapper.deleteByPrimaryKey(session_id);
}

drogon::Task<void> DeleteByAdminId(std::string admin_id) {
  drogon::orm::CoroMapper<AdminSessionsModel> mapper{drogon::app().getFastDbClient()};
  co_await mapper.deleteBy(drogon::orm::Criteria(AdminSessionsModel::Cols::_admin_id, drogon::orm::CompareOperator::EQ, std::move(admin_id)));
}

}  // namespace repo::admin_sessions
