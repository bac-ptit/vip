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

export module repo:registrations;

import domain;

export namespace repo::registrations {

using Registrations = domain::Registrations;
using RegistrationsModel = drogon_model::qlattt::Registrations;

[[nodiscard]] drogon::Task<Registrations> Create(const Registrations& r) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto res{co_await mapper.insert(r)};
  co_return Registrations{std::move(res)};
}

[[nodiscard]] drogon::Task<std::optional<Registrations>> FindById(
    std::string_view id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(std::string{id})};
    co_return Registrations{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::vector<Registrations>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto res{co_await mapper.findAll()};
  std::vector<Registrations> registrations;
  registrations.reserve(res.size());
  for (auto& item : res) {
    registrations.emplace_back(std::move(item));
  }
  co_return registrations;
}

[[nodiscard]] drogon::Task<size_t> Update(const Registrations& r) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  co_return co_await mapper.update(r);
}

[[nodiscard]] drogon::Task<bool> DeleteById(std::string_view id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<RegistrationsModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      RegistrationsModel::Cols::_id, drogon::orm::CompareOperator::EQ,
      std::string{id}))};
  co_return count > 0;
}

}  // namespace repo::registrations
