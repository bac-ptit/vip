#include "media_repository.h"
#include <drogon/drogon.h>
#include <drogon/orm/CoroMapper.h>
#include <model.h>
#include <ranges>

namespace repo::media {

using MediaModel = drogon_model::qlattt::Media;

drogon::Task<domain::Media> Create(const domain::Media& m) {
  drogon::orm::CoroMapper<MediaModel> mapper{drogon::app().getFastDbClient()};
  auto result{co_await mapper.insert(m)};
  co_return domain::Media{std::move(result)};
}

drogon::Task<std::optional<domain::Media>> FindById(std::string mediaId) {
  drogon::orm::CoroMapper<MediaModel> mapper{drogon::app().getFastDbClient()};
  try {
    auto result{co_await mapper.findByPrimaryKey(std::move(mediaId))};
    co_return domain::Media{std::move(result)};
  } catch (...) {
    co_return std::nullopt;
  }
}

drogon::Task<std::vector<domain::Media>> FindAll() {
  drogon::orm::CoroMapper<MediaModel> mapper{drogon::app().getFastDbClient()};
  auto results{co_await mapper.findAll()};
  co_return results |
            std::views::transform([](auto& r) { return domain::Media{std::move(r)}; }) |
            std::ranges::to<std::vector>();
}

drogon::Task<std::vector<domain::Media>> FindActive() {
  drogon::orm::CoroMapper<MediaModel> mapper{drogon::app().getFastDbClient()};
  auto results{co_await mapper.findBy(drogon::orm::Criteria(
      MediaModel::Cols::_is_active, drogon::orm::CompareOperator::EQ, true))};
  co_return results |
            std::views::transform([](auto& r) { return domain::Media{std::move(r)}; }) |
            std::ranges::to<std::vector>();
}

drogon::Task<void> Update(const domain::Media& m) {
  drogon::orm::CoroMapper<MediaModel> mapper{drogon::app().getFastDbClient()};
  co_await mapper.update(m);
}

drogon::Task<bool> DeleteById(std::string mediaId) {
  drogon::orm::CoroMapper<MediaModel> mapper{drogon::app().getFastDbClient()};
  auto count{co_await mapper.deleteBy(drogon::orm::Criteria(
      MediaModel::Cols::_id, drogon::orm::CompareOperator::EQ, std::move(mediaId)))};
  co_return count > 0;
}

}  // namespace repo::media
