#include <utility>
#include <stdexcept>
#include <vector>
#include <optional>
#include <string>
#include <ranges>
#include "media_service.h"

#include <drogon/drogon.h>
#include <models/Media.h>

#include <repositories/repo.h>
#include <dto/dto.h>
#include <domains/domain.h>

using namespace drogon;

namespace {
dto::MediaResponse ToMediaResponse(const domain::Media& media) {
  dto::MediaResponse response;
  response.id = media.getId();
  response.type = media.getType();
  response.title = media.getTitle();
  response.url = media.getUrl();
  response.thumbnail = media.getThumbnail();
  response.sort_order = media.getSortOrder();
  response.is_active = media.getIsActive();
  response.uploaded_by = media.getUploadedBy();
  response.created_at = media.getCreatedAt();
  return response;
}
}  // namespace

drogon::Task<dto::MediaResponse> service::media::Create(
    dto::CreateMediaRequest request, std::optional<std::string> admin_id) {
  domain::Media media;
  media.setType(std::move(request.type));
  if (request.title) media.setTitle(std::move(*request.title));
  media.setUrl(std::move(request.url));
  if (request.thumbnail) media.setThumbnail(std::move(*request.thumbnail));
  media.setSortOrder(request.sort_order);
  media.setIsActive(request.is_active);
  if (admin_id) media.setUploadedBy(std::move(*admin_id));

  auto created{co_await repo::media::Create(media)};
  co_return ToMediaResponse(created);
}

drogon::Task<void> service::media::Update(
    std::string media_id, dto::UpdateMediaRequest request) {
  auto media_opt{co_await repo::media::FindById(media_id)};
  if (!media_opt) {
    throw std::runtime_error{"Media not found"};
  }

  auto existing{std::move(*media_opt)};

  if (request.type) existing.setType(std::move(*request.type));
  if (request.title) existing.setTitle(std::move(*request.title));
  if (request.url) existing.setUrl(std::move(*request.url));
  if (request.thumbnail) existing.setThumbnail(std::move(*request.thumbnail));
  if (request.sort_order) existing.setSortOrder(*request.sort_order);
  if (request.is_active) existing.setIsActive(*request.is_active);

  co_await repo::media::Update(existing);
}

drogon::Task<void> service::media::Delete(std::string media_id) {
  co_await repo::media::DeleteById(std::move(media_id));
}

drogon::Task<std::optional<dto::MediaResponse>> service::media::GetById(
    std::string media_id) {
  auto media_opt{co_await repo::media::FindById(std::move(media_id))};
  if (media_opt) {
    co_return ToMediaResponse(std::move(*media_opt));
  }
  co_return std::nullopt;
}

drogon::Task<std::vector<dto::MediaResponse>> service::media::GetAll() {
  auto medias_raw{co_await repo::media::FindAll()};
  std::vector<dto::MediaResponse> result;
  result.reserve(medias_raw.size());
  
  std::ranges::transform(medias_raw, std::back_inserter(result), ToMediaResponse);
  
  co_return result;
}

drogon::Task<std::vector<dto::MediaResponse>> service::media::GetActiveMedia() {
  auto medias_raw{co_await repo::media::FindActive()};
  std::vector<dto::MediaResponse> result;
  result.reserve(medias_raw.size());
  
  std::ranges::transform(medias_raw, std::back_inserter(result), ToMediaResponse);
  
  co_return result;
}
