//
// Created by bac on 3/27/26.
//
module;
#include <drogon/drogon.h>
#include <model.h>
#include <drogon/orm/CoroMapper.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

module repo;

import domain;

using namespace drogon;
using namespace drogon::orm;

drogon::Task<std::optional<domain::Admin>> repo::admin::FindById(
    std::string_view id) {
  try {
    auto db{app().getFastDbClient()};
    CoroMapper<drogon_model::qlattt::Admins> mapper(db);
    auto admin{co_await mapper.findByPrimaryKey(std::string(id))};
    co_return domain::Admin{std::move(admin)};
  } catch (const DrogonDbException&) {
    co_return std::nullopt;
  }
}

drogon::Task<std::optional<domain::Admin>> repo::admin::FindByUsername(
    std::string_view username) {
  try {
    auto db{app().getFastDbClient()};
    CoroMapper<drogon_model::qlattt::Admins> mapper(db);
    auto admins{co_await mapper.findBy(
        Criteria(drogon_model::qlattt::Admins::Cols::_username,
                 CompareOperator::EQ, std::string(username)))};
    if (admins.empty()) {
      co_return std::nullopt;
    }
    co_return domain::Admin{std::move(admins[0])};
  } catch (const DrogonDbException&) {
    co_return std::nullopt;
  }
}

drogon::Task<std::vector<domain::Admin>> repo::admin::FindAll() {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto admins{co_await mapper.findAll()};
  std::vector<domain::Admin> domain_admins;
  domain_admins.reserve(admins.size());
  for (auto& a : admins) {
    domain_admins.emplace_back(std::move(a));
  }
  co_return domain_admins;
}

drogon::Task<std::vector<domain::Admin>> repo::admin::FindActiveAdmins() {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto admins{co_await mapper.findBy(
      Criteria(drogon_model::qlattt::Admins::Cols::_is_active,
               CompareOperator::EQ, true))};
  std::vector<domain::Admin> domain_admins;
  domain_admins.reserve(admins.size());
  for (auto& a : admins) {
    domain_admins.emplace_back(std::move(a));
  }
  co_return domain_admins;
}

drogon::Task<domain::Admin> repo::admin::Create(const Admin& admin) {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto created_admin{co_await mapper.insert(admin)};
  co_return domain::Admin{std::move(created_admin)};
}

drogon::Task<size_t> repo::admin::Update(const Admin& admin) {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto count{co_await mapper.update(admin)};
  co_return count;
}

drogon::Task<size_t> repo::admin::DeleteById(std::string_view id) {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto count{co_await mapper.deleteByPrimaryKey(std::string(id))};
  co_return count;
}

drogon::Task<bool> repo::admin::SoftDeleteById(std::string_view id) {
  auto admin{co_await FindById(id)};
  if (!admin) {
    co_return false;
  }
  admin->setIsActive(false);
  auto count{co_await Update(*admin)};
  co_return count > 0;
}

drogon::Task<bool> repo::admin::ExistsByUsername(std::string_view username) {
  try {
    auto db{app().getFastDbClient()};
    CoroMapper<drogon_model::qlattt::Admins> mapper(db);
    auto count{co_await mapper.count(
        Criteria(drogon_model::qlattt::Admins::Cols::_username,
                 CompareOperator::EQ, std::string(username)))};
    co_return count > 0;
  } catch (const DrogonDbException&) {
    co_return false;
  }
}

drogon::Task<bool> repo::admin::ExistsById(std::string_view id) {
  try {
    auto db{app().getFastDbClient()};
    CoroMapper<drogon_model::qlattt::Admins> mapper(db);
    auto count{co_await mapper.count(
        Criteria(drogon_model::qlattt::Admins::Cols::_id, CompareOperator::EQ,
                 std::string(id)))};
    co_return count > 0;
  } catch (const DrogonDbException&) {
    co_return false;
  }
}

drogon::Task<size_t> repo::admin::CountAll() {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto count{co_await mapper.count()};
  co_return count;
}

drogon::Task<size_t> repo::admin::CountActive() {
  auto db{app().getFastDbClient()};
  CoroMapper<drogon_model::qlattt::Admins> mapper(db);
  auto count{co_await mapper.count(
      Criteria(drogon_model::qlattt::Admins::Cols::_is_active,
               CompareOperator::EQ, true))};
  co_return count;
}
