#pragma once
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::project_info {

using ProjectInfo = domain::ProjectInfo;
using ProjectInfoModel = drogon_model::qlattt::ProjectInfo;

[[nodiscard]] inline drogon::Task<ProjectInfo> Create(ProjectInfo p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto res{co_await mapper.insert(p)};
  co_return ProjectInfo{std::move(res)};
}

[[nodiscard]] inline drogon::Task<std::optional<ProjectInfo>> FindById(
    std::string id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(id)};
    co_return ProjectInfo{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] inline drogon::Task<std::vector<ProjectInfo>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto res{co_await mapper.findAll()};

  co_return res | std::views::transform([](auto&& item) {
    return ProjectInfo{std::move(item)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<size_t> Update(ProjectInfo p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  co_return co_await mapper.update(p);
}

[[nodiscard]] inline drogon::Task<bool> DeleteById(std::string id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      ProjectInfoModel::Cols::_id, drogon::orm::CompareOperator::EQ,
      std::move(id)))};
  co_return count > 0;
}

}  // namespace repo::project_info
