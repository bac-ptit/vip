#include "project_info_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::project_info {

using ProjectInfoModel = drogon_model::qlattt::ProjectInfo;

drogon::Task<domain::ProjectInfo> Create(domain::ProjectInfo project_info) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto res{co_await mapper.insert(project_info)};
  co_return domain::ProjectInfo{std::move(res)};
}

drogon::Task<std::optional<domain::ProjectInfo>> FindById(std::string identifier) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(identifier)};
    co_return domain::ProjectInfo{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::vector<domain::ProjectInfo>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto res{co_await mapper.findAll()};
  
  co_return res | std::views::transform([](auto& item) {
    return domain::ProjectInfo{std::move(item)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<size_t> Update(domain::ProjectInfo project_info) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  co_return co_await mapper.update(project_info);
}

drogon::Task<bool> DeleteById(std::string identifier) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      ProjectInfoModel::Cols::_id, drogon::orm::CompareOperator::EQ,
      std::move(identifier)))};
  co_return count > 0;
}

}  // namespace repo::project_info
