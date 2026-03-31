#pragma once
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::registration_interests {

[[nodiscard]] inline drogon::Task<domain::RegistrationInterests> Create(
    domain::RegistrationInterests ri) {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{
      drogon::app().getDbClient()};
  auto result{co_await mapper.insert(ri)};
  co_return domain::RegistrationInterests{std::move(result)};
}

[[nodiscard]] inline drogon::Task<std::vector<domain::RegistrationInterests>>
FindByRegistrationId(std::string registration_id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{
      drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(
      drogon_model::qlattt::RegistrationInterests::Cols::_registration_id,
      drogon::orm::CompareOperator::EQ, registration_id))};

  co_return results | std::views::transform([](auto&& r) {
    return domain::RegistrationInterests{std::move(r)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<void> DeleteByRegistrationId(
    std::string registration_id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{
      drogon::app().getDbClient()};
  co_await mapper.deleteBy(drogon::orm::Criteria(
      drogon_model::qlattt::RegistrationInterests::Cols::_registration_id,
      drogon::orm::CompareOperator::EQ, std::move(registration_id)));
}

[[nodiscard]] inline drogon::Task<std::vector<domain::RegistrationInterests>> FindAll() {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{
      drogon::app().getDbClient()};
  auto results{co_await mapper.findAll()};

  co_return results | std::views::transform([](auto&& r) {
    return domain::RegistrationInterests{std::move(r)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<bool> DeleteById(std::string id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{
      drogon::app().getDbClient()};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      drogon_model::qlattt::RegistrationInterests::Cols::_id,
      drogon::orm::CompareOperator::EQ, std::move(id)))};
  co_return count > 0;
}

}  // namespace repo::registration_interests
