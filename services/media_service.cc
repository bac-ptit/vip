//
// Created by bac on 3/28/26.
//
module;
#include <drogon/drogon.h>
#include <models/Media.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

module service;

import repo;
import dto;
import domain;

using namespace drogon;

namespace {
dto::MediaResponse ToMediaResponse(const domain::Media& m) {
  dto::MediaResponse resp;
  resp.id = m.getId();
  resp.type = m.getType();
  resp.title = m.getTitle();
  resp.url = m.getUrl();
  resp.thumbnail = m.getThumbnail();
  resp.sort_order = m.getSortOrder();
  resp.is_active = m.getIsActive();
  resp.uploaded_by = m.getUploadedBy();
  resp.created_at = m.getCreatedAt();
  return resp;
}
}  // namespace

drogon::Task<dto::MediaResponse> service::media::Create(
    dto::CreateMediaRequest request, std::optional<std::string> admin_id) {
  domain::Media m;
  m.setType(std::move(request.type));
  if (request.title) m.setTitle(std::move(*request.title));
  m.setUrl(std::move(request.url));
  if (request.thumbnail) m.setThumbnail(std::move(*request.thumbnail));
  m.setSortOrder(request.sort_order);
  m.setIsActive(request.is_active);
  if (admin_id) m.setUploadedBy(std::move(*admin_id));

  auto created{co_await repo::media::Create(m)};
  co_return ToMediaResponse(created);
}

drogon::Task<void> service::media::Update(
    std::string id, dto::UpdateMediaRequest request) {
  auto existing_opt{co_await repo::media::FindById(id)};
  if (!existing_opt) {
    throw std::runtime_error{"Media not found"};
  }

  auto existing{std::move(*existing_opt)};

  if (request.type) existing.setType(std::move(*request.type));
  if (request.title) existing.setTitle(std::move(*request.title));
  if (request.url) existing.setUrl(std::move(*request.url));
  if (request.thumbnail) existing.setThumbnail(std::move(*request.thumbnail));
  if (request.sort_order) existing.setSortOrder(*request.sort_order);
  if (request.is_active) existing.setIsActive(*request.is_active);

  co_await repo::media::Update(existing);
}

drogon::Task<void> service::media::Delete(std::string id) {
  co_await repo::media::DeleteById(std::move(id));
}

drogon::Task<std::optional<dto::MediaResponse>> service::media::GetById(
    std::string id) {
  auto m_opt{co_await repo::media::FindById(std::move(id))};
  if (m_opt) {
    co_return ToMediaResponse(std::move(*m_opt));
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
