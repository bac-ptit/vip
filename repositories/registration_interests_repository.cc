#include "registration_interests_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::registration_interests {

using RegistrationInterestsModel = drogon_model::qlattt::RegistrationInterests;

drogon::Task<domain::RegistrationInterests> Create(domain::RegistrationInterests ri) {
  drogon::orm::CoroMapper<RegistrationInterestsModel> mapper{drogon::app().getDbClient()};
  auto result{co_await mapper.insert(ri)};
  co_return domain::RegistrationInterests{std::move(result)};
}

drogon::Task<std::vector<domain::RegistrationInterests>> FindByRegistrationId(std::string registration_id) {
  drogon::orm::CoroMapper<RegistrationInterestsModel> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(RegistrationInterestsModel::Cols::_registration_id, drogon::orm::CompareOperator::EQ, registration_id))};
  
  co_return results | std::views::transform([](auto& r) {
    return domain::RegistrationInterests{std::move(r)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<void> DeleteByRegistrationId(std::string registration_id) {
  drogon::orm::CoroMapper<RegistrationInterestsModel> mapper{drogon::app().getDbClient()};
  co_await mapper.deleteBy(drogon::orm::Criteria(RegistrationInterestsModel::Cols::_registration_id, drogon::orm::CompareOperator::EQ, std::move(registration_id)));
}

drogon::Task<std::vector<domain::RegistrationInterests>> FindAll() {
  drogon::orm::CoroMapper<RegistrationInterestsModel> mapper{drogon::app().getDbClient()};
  auto results{co_await mapper.findAll()};
  
  co_return results | std::views::transform([](auto& r) {
    return domain::RegistrationInterests{std::move(r)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<bool> DeleteById(std::string registrationId) {
  drogon::orm::CoroMapper<RegistrationInterestsModel> mapper{drogon::app().getDbClient()};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(RegistrationInterestsModel::Cols::_id, drogon::orm::CompareOperator::EQ, std::move(registrationId)))};
  co_return count > 0;
}

}  // namespace repo::registration_interests
