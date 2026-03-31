#pragma once
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::media {

[[nodiscard]] inline drogon::Task<domain::Media> Create(const domain::Media& m) {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  auto result{co_await mapper.insert(m)};
  co_return domain::Media{std::move(result)};
}

[[nodiscard]] inline drogon::Task<std::optional<domain::Media>> FindById(std::string id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  try {
    auto result{co_await mapper.findByPrimaryKey(std::move(id))};
    co_return domain::Media{std::move(result)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] inline drogon::Task<std::vector<domain::Media>> FindAll() {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findAll()};
  co_return results |
            std::views::transform([](auto& r) { return domain::Media{std::move(r)}; }) |
            std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<std::vector<domain::Media>> FindActive() {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(
      drogon_model::qlattt::Media::Cols::_is_active,
      drogon::orm::CompareOperator::EQ, true))};
  co_return results |
            std::views::transform([](auto& r) { return domain::Media{std::move(r)}; }) |
            std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<void> Update(const domain::Media& m) {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  co_await mapper.update(m);
}

[[nodiscard]] inline drogon::Task<bool> DeleteById(std::string id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      drogon_model::qlattt::Media::Cols::_id,
      drogon::orm::CompareOperator::EQ, std::move(id)))};
  co_return count > 0;
}

}  // namespace repo::media
