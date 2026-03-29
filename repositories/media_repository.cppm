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

export module repo:media;

import domain;

export namespace repo::media {

[[nodiscard]] drogon::Task<domain::Media> Create(const domain::Media& m) {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  auto result{co_await mapper.insert(m)};
  co_return domain::Media{std::move(result)};
}

[[nodiscard]] drogon::Task<std::optional<domain::Media>> FindById(std::string_view id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  try {
    auto result{co_await mapper.findByPrimaryKey(std::string{id})};
    co_return domain::Media{std::move(result)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::vector<domain::Media>> FindAll() {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findAll()};
  std::vector<domain::Media> domain_results;
  domain_results.reserve(results.size());
  for (auto& r : results) {
    domain_results.emplace_back(std::move(r));
  }
  co_return domain_results;
}

[[nodiscard]] drogon::Task<std::vector<domain::Media>> FindActive() {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(drogon_model::qlattt::Media::Cols::_is_active, drogon::orm::CompareOperator::EQ, true))};
  std::vector<domain::Media> domain_results;
  domain_results.reserve(results.size());
  for (auto& r : results) {
    domain_results.emplace_back(std::move(r));
  }
  co_return domain_results;
}

[[nodiscard]] drogon::Task<void> Update(const domain::Media& m) {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  co_await mapper.update(m);
}

[[nodiscard]] drogon::Task<bool> DeleteById(std::string_view id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::Media> mapper{drogon::app().getDbClient()};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(drogon_model::qlattt::Media::Cols::_id, drogon::orm::CompareOperator::EQ, std::string{id}))};
  co_return count > 0;
}

}  // namespace repo::media
