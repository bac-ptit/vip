#include "admin_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::admin {

using AdminsModel = drogon_model::qlattt::Admins;

drogon::Task<std::optional<Admin>> FindById(std::string admin_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(admin_id)};
    co_return Admin{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::optional<Admin>> FindByUsername(std::string username) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  auto res{co_await mapper.findBy(drogon::orm::Criteria(
      AdminsModel::Cols::_username, drogon::orm::CompareOperator::EQ,
      std::move(username)))};

  if (res.empty()) {
    co_return std::nullopt;
  }
  co_return Admin{std::move(res[0])};
}

drogon::Task<std::vector<Admin>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  auto res{co_await mapper.findAll()};

  co_return res | std::views::transform([](auto& item) {
    return Admin{std::move(item)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<std::vector<Admin>> FindActiveAdmins() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  auto res{co_await mapper.findBy(drogon::orm::Criteria(
      AdminsModel::Cols::_is_active, drogon::orm::CompareOperator::EQ, true))};

  co_return res | std::views::transform([](auto& item) {
    return Admin{std::move(item)};
  }) | std::ranges::to<std::vector>();
}

drogon::Task<Admin> Create(const Admin& admin) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  auto res{co_await mapper.insert(admin)};
  co_return Admin{std::move(res)};
}

drogon::Task<size_t> Update(const Admin& admin) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  co_return co_await mapper.update(admin);
}

drogon::Task<size_t> DeleteById(std::string admin_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  co_return co_await mapper.deleteByPrimaryKey(std::move(admin_id));
}

drogon::Task<bool> SoftDeleteById(std::string admin_id) {
  auto admin_opt{co_await FindById(admin_id)};
  if (!admin_opt) co_return false;
  admin_opt->setIsActive(false);
  co_return (co_await Update(*admin_opt)) > 0;
}

drogon::Task<bool> ExistsByUsername(std::string username) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  auto count{co_await mapper.count(
      drogon::orm::Criteria(AdminsModel::Cols::_username,
                            drogon::orm::CompareOperator::EQ,
                            std::move(username)))};
  co_return count > 0;
}

drogon::Task<bool> ExistsById(std::string admin_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  auto count{co_await mapper.count(
      drogon::orm::Criteria(AdminsModel::Cols::_id,
                            drogon::orm::CompareOperator::EQ,
                            std::move(admin_id)))};
  co_return count > 0;
}

drogon::Task<size_t> CountAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  co_return co_await mapper.count();
}

drogon::Task<size_t> CountActive() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<AdminsModel> mapper{db};
  co_return co_await mapper.count(drogon::orm::Criteria(
      AdminsModel::Cols::_is_active, drogon::orm::CompareOperator::EQ, true));
}

}  // namespace repo::admin
