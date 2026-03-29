//
// Created by bac on 3/28/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <string>
#include <vector>

export module repo:project_info;

import domain;

export namespace repo::project_info {

using ProjectInfo = domain::ProjectInfo;
using ProjectInfoModel = drogon_model::qlattt::ProjectInfo;

[[nodiscard]] drogon::Task<ProjectInfo> Create(const ProjectInfo& p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto res{co_await mapper.insert(p)};
  co_return ProjectInfo{std::move(res)};
}

[[nodiscard]] drogon::Task<std::optional<ProjectInfo>> FindById(
    std::string_view id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(std::string{id})};
    co_return ProjectInfo{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::vector<ProjectInfo>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto res{co_await mapper.findAll()};
  std::vector<ProjectInfo> project_infos;
  project_infos.reserve(res.size());
  for (auto& item : res) {
    project_infos.emplace_back(std::move(item));
  }
  co_return project_infos;
}

[[nodiscard]] drogon::Task<size_t> Update(const ProjectInfo& p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  co_return co_await mapper.update(p);
}

[[nodiscard]] drogon::Task<bool> DeleteById(std::string_view id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProjectInfoModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      ProjectInfoModel::Cols::_id, drogon::orm::CompareOperator::EQ,
      std::string{id}))};
  co_return count > 0;
}

}  // namespace repo::project_info
