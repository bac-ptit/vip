//
// Created by bac on 3/27/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

export module repo:product;

import domain;

export namespace repo::product {

using Product = domain::Product;
using ProductsModel = drogon_model::qlattt::Products;

[[nodiscard]] drogon::Task<Product> Create(const Product& p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  auto res{co_await mapper.insert(p)};
  co_return Product{std::move(res)};
}

[[nodiscard]] drogon::Task<std::optional<Product>> FindById(std::string id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(std::move(id))};
    co_return Product{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] drogon::Task<std::vector<Product>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  auto res{co_await mapper.findAll()};
  co_return res | std::views::transform([](auto& item) { return Product{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

[[nodiscard]] drogon::Task<std::vector<Product>> FindActive() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  auto res{co_await mapper.findBy(drogon::orm::Criteria(
      ProductsModel::Cols::_is_active, drogon::orm::CompareOperator::EQ, true))};
  co_return res | std::views::transform([](auto& item) { return Product{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

[[nodiscard]] drogon::Task<size_t> Update(const Product& p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  co_return co_await mapper.update(p);
}

[[nodiscard]] drogon::Task<bool> DeleteById(std::string id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductsModel> mapper{db};
  auto count{co_await mapper.deleteBy(
      drogon::orm::Criteria(ProductsModel::Cols::_id,
                            drogon::orm::CompareOperator::EQ, std::move(id)))};
  co_return count > 0;
}

}  // namespace repo::product
