//
// Created by bac on 3/27/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <string>
#include <vector>

export module repo:registration_interests;

import domain;

export namespace repo::registration_interests {

[[nodiscard]] drogon::Task<domain::RegistrationInterests> Create(const domain::RegistrationInterests& ri) {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{drogon::app().getDbClient()};
  auto result{co_await mapper.insert(ri)};
  co_return domain::RegistrationInterests{std::move(result)};
}

[[nodiscard]] drogon::Task<std::vector<domain::RegistrationInterests>> FindByRegistrationId(std::string_view registration_id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(drogon_model::qlattt::RegistrationInterests::Cols::_registration_id, drogon::orm::CompareOperator::EQ, std::string{registration_id}))};
  std::vector<domain::RegistrationInterests> domain_results;
  domain_results.reserve(results.size());
  for (auto& r : results) {
    domain_results.emplace_back(std::move(r));
  }
  co_return domain_results;
}

[[nodiscard]] drogon::Task<void> DeleteByRegistrationId(std::string_view registration_id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{drogon::app().getDbClient()};
  co_await mapper.deleteBy(drogon::orm::Criteria(drogon_model::qlattt::RegistrationInterests::Cols::_registration_id, drogon::orm::CompareOperator::EQ, std::string{registration_id}));
}

[[nodiscard]] drogon::Task<std::vector<domain::RegistrationInterests>> FindAll() {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findAll()};
  std::vector<domain::RegistrationInterests> domain_results;
  domain_results.reserve(results.size());
  for (auto& r : results) {
    domain_results.emplace_back(std::move(r));
  }
  co_return domain_results;
}

[[nodiscard]] drogon::Task<bool> DeleteById(std::string_view id) {
  drogon::orm::CoroMapper<drogon_model::qlattt::RegistrationInterests> mapper{drogon::app().getDbClient()};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(drogon_model::qlattt::RegistrationInterests::Cols::_id, drogon::orm::CompareOperator::EQ, std::string{id}))};
  co_return count > 0;
}

}  // namespace repo::registration_interests
