#pragma once
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <ranges>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::registrations {

using Registrations = domain::Registrations;
using RegistrationsModel = drogon_model::qlattt::Registrations;

[[nodiscard]] inline drogon::Task<Registrations> Create(Registrations r) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto res{co_await mapper.insert(r)};
  co_return Registrations{std::move(res)};
}

[[nodiscard]] inline drogon::Task<std::optional<Registrations>> FindById(
    std::string id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(id)};
    co_return Registrations{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] inline drogon::Task<std::vector<Registrations>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto res{co_await mapper.findAll()};

  co_return res | std::views::transform([](auto&& item) {
    return Registrations{std::move(item)};
  }) | std::ranges::to<std::vector>();
}

[[nodiscard]] inline drogon::Task<size_t> Update(Registrations r) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  co_return co_await mapper.update(r);
}

[[nodiscard]] inline drogon::Task<bool> DeleteById(std::string id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      RegistrationsModel::Cols::_id, drogon::orm::CompareOperator::EQ,
      std::move(id)))};
  co_return count > 0;
}

}  // namespace repo::registrations
