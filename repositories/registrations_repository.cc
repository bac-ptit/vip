#include "registrations_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::registrations {

using RegistrationsModel = drogon_model::qlattt::Registrations;

drogon::Task<domain::Registrations> Create(domain::Registrations registration) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto res{co_await mapper.insert(registration)};
  co_return domain::Registrations{std::move(res)};
}

drogon::Task<std::optional<domain::Registrations>> FindById(std::string registration_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(registration_id)};
    co_return domain::Registrations{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::vector<domain::Registrations>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto res{co_await mapper.findAll()};
  
  co_return res | std::views::transform([](auto& item) {
    return domain::Registrations{std::move(item)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<size_t> Update(domain::Registrations registration) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  co_return co_await mapper.update(registration);
}

drogon::Task<bool> DeleteById(std::string registration_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      RegistrationsModel::Cols::_id, drogon::orm::CompareOperator::EQ,
      std::move(registration_id)))};
  co_return count > 0;
}

}  // namespace repo::registrations
