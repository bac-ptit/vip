#include "product_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::product {

using ProductsModel = drogon_model::qlattt::Products;

drogon::Task<domain::Product> Create(const domain::Product& p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  auto res{co_await mapper.insert(p)};
  co_return domain::Product{std::move(res)};
}

drogon::Task<std::optional<domain::Product>> FindById(std::string productId) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(std::move(productId))};
    co_return domain::Product{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::vector<domain::Product>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  auto res{co_await mapper.findAll()};
  co_return res | std::views::transform([](auto& item) { return domain::Product{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

drogon::Task<std::vector<domain::Product>> FindActive() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  auto res{co_await mapper.findBy(drogon::orm::Criteria(
      ProductsModel::Cols::_is_active, drogon::orm::CompareOperator::EQ, true))};
  co_return res | std::views::transform([](auto& item) { return domain::Product{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

drogon::Task<size_t> Update(const domain::Product& p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  co_return co_await mapper.update(p);
}

drogon::Task<bool> DeleteById(std::string productId) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  auto count{co_await mapper.deleteBy(
      drogon::orm::Criteria(ProductsModel::Cols::_id,
                            drogon::orm::CompareOperator::EQ, std::move(productId)))};
  co_return count > 0;
}

}  // namespace repo::product
