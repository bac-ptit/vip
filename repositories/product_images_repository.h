#pragma once
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <optional>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::product_images {

using ProductImages = domain::ProductImages;
using ProductImagesModel = drogon_model::qlattt::ProductImages;

[[nodiscard]] inline drogon::Task<ProductImages> Create(const ProductImages& p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  auto res{co_await mapper.insert(p)};
  co_return ProductImages{std::move(res)};
}

[[nodiscard]] inline drogon::Task<std::optional<ProductImages>> FindById(
    std::string_view id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(std::string{id})};
    co_return ProductImages{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

[[nodiscard]] inline drogon::Task<std::vector<ProductImages>> FindByProductId(
    std::string_view product_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  auto res{co_await mapper.findBy(drogon::orm::Criteria(
      ProductImagesModel::Cols::_product_id, drogon::orm::CompareOperator::EQ,
      std::string{product_id}))};
  std::vector<ProductImages> images;
  images.reserve(res.size());
  for (auto& item : res) {
    images.emplace_back(std::move(item));
  }
  co_return images;
}

[[nodiscard]] inline drogon::Task<std::vector<ProductImages>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  auto res{co_await mapper.findAll()};
  std::vector<ProductImages> images;
  images.reserve(res.size());
  for (auto& item : res) {
    images.emplace_back(std::move(item));
  }
  co_return images;
}

[[nodiscard]] inline drogon::Task<size_t> Update(const ProductImages& p) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  co_return co_await mapper.update(p);
}

[[nodiscard]] inline drogon::Task<bool> DeleteById(std::string_view id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      ProductImagesModel::Cols::_id, drogon::orm::CompareOperator::EQ,
      std::string{id}))};
  co_return count > 0;
}

}  // namespace repo::product_images
