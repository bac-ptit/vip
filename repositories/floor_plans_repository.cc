#include "floor_plans_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::floor_plans {

using FloorPlansModel = drogon_model::qlattt::FloorPlans;

drogon::Task<domain::FloorPlans> Create(const domain::FloorPlans& f) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  auto res{co_await mapper.insert(f)};
  co_return domain::FloorPlans{std::move(res)};
}

drogon::Task<std::optional<domain::FloorPlans>> FindById(std::string floorPlanId) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(std::move(floorPlanId))};
    co_return domain::FloorPlans{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::vector<domain::FloorPlans>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  auto res{co_await mapper.findAll()};
  co_return res |
            std::views::transform([](auto& item) { return domain::FloorPlans{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

drogon::Task<std::vector<domain::FloorPlans>> FindActive() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  auto res{co_await mapper.findBy(drogon::orm::Criteria(
      FloorPlansModel::Cols::_is_active, drogon::orm::CompareOperator::EQ, true))};
  co_return res |
            std::views::transform([](auto& item) { return domain::FloorPlans{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

drogon::Task<size_t> Update(const domain::FloorPlans& f) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  co_return co_await mapper.update(f);
}

drogon::Task<bool> DeleteById(std::string floorPlanId) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<FloorPlansModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      FloorPlansModel::Cols::_id, drogon::orm::CompareOperator::EQ, std::move(floorPlanId)))};
  co_return count > 0;
}

}  // namespace repo::floor_plans
