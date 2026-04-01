#include "product_images_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::product_images {

using ProductImagesModel = drogon_model::qlattt::ProductImages;

drogon::Task<domain::ProductImages> Create(const domain::ProductImages& product_images) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  auto res{co_await mapper.insert(product_images)};
  co_return domain::ProductImages{std::move(res)};
}

drogon::Task<std::optional<domain::ProductImages>> FindById(std::string_view image_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  try {
    auto res{co_await mapper.findByPrimaryKey(std::string{image_id})};
    co_return domain::ProductImages{std::move(res)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::vector<domain::ProductImages>> FindByProductId(std::string_view product_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  auto res{co_await mapper.findBy(drogon::orm::Criteria(
      ProductImagesModel::Cols::_product_id, drogon::orm::CompareOperator::EQ,
      std::string{product_id}))};
  
  co_return res |
            std::views::transform([](auto& item) { return domain::ProductImages{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

drogon::Task<std::vector<domain::ProductImages>> FindAll() {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  auto res{co_await mapper.findAll()};
  
  co_return res |
            std::views::transform([](auto& item) { return domain::ProductImages{std::move(item)}; }) |
            std::ranges::to<std::vector>();
}

drogon::Task<size_t> Update(const domain::ProductImages& product_images) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  co_return co_await mapper.update(product_images);
}

drogon::Task<bool> DeleteById(std::string_view image_id) {
  auto db{drogon::app().getFastDbClient()};
  drogon::orm::CoroMapper<ProductImagesModel> mapper{db};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      ProductImagesModel::Cols::_id, drogon::orm::CompareOperator::EQ,
      std::string{image_id}))};
  co_return count > 0;
}

}  // namespace repo::product_images
