//
// Created by bac on 3/28/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

export module repo:floor_plans;

import domain;

export namespace repo::floor_plans {

using FloorPlans = domain::FloorPlans;
using FloorPlansModel = drogon_model::qlattt::FloorPlans;

[[nodiscard]] drogon::Task<FloorPlans> Create(const FloorPlans& f) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  auto res{co_await mapper.insert(f)};
  co_return FloorPlans{std::move(res)};
}

[[nodiscard]] drogon::Task<std::optional<FloorPlans>> FindById(std::string id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(std::move(id))};
    co_return FloorPlans{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::vector<FloorPlans>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  auto res{co_await mapper.findAll()};
  co_return res |
            std::views::transform([](auto& item) { return FloorPlans{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

[[nodiscard]] drogon::Task<std::vector<FloorPlans>> FindActive() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  auto res{co_await mapper.findBy(drogon::orm::Criteria(
      FloorPlansModel::Cols::_is_active, drogon::orm::CompareOperator::EQ, true))};
  co_return res |
            std::views::transform([](auto& item) { return FloorPlans{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

[[nodiscard]] drogon::Task<size_t> Update(const FloorPlans& f) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  co_return co_await mapper.update(f);
}

[[nodiscard]] drogon::Task<bool> DeleteById(std::string id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      FloorPlansModel::Cols::_id, drogon::orm::CompareOperator::EQ, std::move(id)))};
  co_return count > 0;
}

}  // namespace repo::floor_plans
