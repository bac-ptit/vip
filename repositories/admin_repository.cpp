//
// Created by bac on 3/27/26.
//
module;
#include <drogon/drogon.h>
#include <model.h>
#include <drogon/orm/CoroMapper.h>
import std;

module repo;

import domain;

using namespace drogon;
using namespace drogon::orm;

[[nodiscard]] drogon::Task<std::optional<domain::Admin>> repo::admin::FindById(
    std::string id) {
  try {
    auto db{app().getFastDbClient()};
    CoroMapper<drogon_model::qlattt::Admins> mapper(db);
    auto admin{co_await mapper.findByPrimaryKey(std::move(id))};
    co_return domain::Admin{std::move(admin)};
  } catch (const DrogonDbException&) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::optional<domain::Admin>> repo::admin::FindByUsername(
    std::string username) {
  try {
    auto db{app().getFastDbClient()};
    CoroMapper<drogon_model::qlattt::Admins> mapper(db);
    auto admins{co_await mapper.findBy(
        Criteria(drogon_model::qlattt::Admins::Cols::_username,
                 CompareOperator::EQ, std::move(username)))};
    if (admins.empty()) {
      co_return std::nullopt;
    }
    co_return domain::Admin{std::move(admins[0])};
  } catch (const DrogonDbException&) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::vector<domain::Admin>> repo::admin::FindAll() {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto admins{co_await mapper.findAll()};
  co_return admins | std::views::transform([](auto& a) { return domain::Admin{std::move(a)}; }) |
            std::ranges::to<std::vector>();
}

[[nodiscard]] drogon::Task<std::vector<domain::Admin>> repo::admin::FindActiveAdmins() {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto admins{co_await mapper.findBy(
      Criteria(drogon_model::qlattt::Admins::Cols::_is_active,
               CompareOperator::EQ, true))};
  co_return admins | std::views::transform([](auto& a) { return domain::Admin{std::move(a)}; }) |
            std::ranges::to<std::vector>();
}

[[nodiscard]] drogon::Task<domain::Admin> repo::admin::Create(const Admin& admin) {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto created_admin{co_await mapper.insert(admin)};
  co_return domain::Admin{std::move(created_admin)};
}

[[nodiscard]] drogon::Task<size_t> repo::admin::Update(const Admin& admin) {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto count{co_await mapper.update(admin)};
  co_return count;
}

[[nodiscard]] drogon::Task<size_t> repo::admin::DeleteById(std::string id) {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto count{co_await mapper.deleteByPrimaryKey(std::move(id))};
  co_return count;
}

[[nodiscard]] drogon::Task<bool> repo::admin::SoftDeleteById(std::string id) {
  auto admin{co_await FindById(id)};
  if (!admin) {
    co_return false;
  }
  admin->setIsActive(false);
  auto count{co_await Update(*admin)};
  co_return count > 0;
}

[[nodiscard]] drogon::Task<bool> repo::admin::ExistsByUsername(std::string username) {
  try {
    auto db{app().getFastDbClient()};
    CoroMapper<drogon_model::qlattt::Admins> mapper(db);
    auto count{co_await mapper.count(
        Criteria(drogon_model::qlattt::Admins::Cols::_username,
                 CompareOperator::EQ, std::move(username)))};
    co_return count > 0;
  } catch (const DrogonDbException&) {
    co_return false;
  }
}

[[nodiscard]] drogon::Task<bool> repo::admin::ExistsById(std::string id) {
  try {
    auto db{app().getFastDbClient()};
    CoroMapper<drogon_model::qlattt::Admins> mapper(db);
    auto count{co_await mapper.count(
        Criteria(drogon_model::qlattt::Admins::Cols::_id, CompareOperator::EQ,
                 std::move(id)))};
    co_return count > 0;
  } catch (const DrogonDbException&) {
    co_return false;
  }
}

[[nodiscard]] drogon::Task<size_t> repo::admin::CountAll() {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto count{co_await mapper.count()};
  co_return count;
}

[[nodiscard]] drogon::Task<size_t> repo::admin::CountActive() {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto count{co_await mapper.count(
      Criteria(drogon_model::qlattt::Admins::Cols::_is_active,
               CompareOperator::EQ, true))};
  co_return count;
}
