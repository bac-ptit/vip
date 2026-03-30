//
// Created by bac on 3/28/26.
//
module;
#include <drogon/drogon.h>
#include <models/ProjectInfo.h>
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
dto::ProjectInfoResponse ToProjectInfoResponse(const domain::ProjectInfo& p) {
  dto::ProjectInfoResponse resp;
  resp.id = p.getId();
  resp.name = p.getName();
  resp.developer = p.getDeveloper();
  resp.scale = p.getScale();
  resp.description = p.getDescription();
  resp.map_lat = p.getMapLat();
  resp.map_lng = p.getMapLng();
  resp.map_zoom = p.getMapZoom();
  resp.updated_by = p.getUpdatedBy();
  resp.updated_at = p.getUpdatedAt();
  return resp;
}
}  // namespace

drogon::Task<dto::ProjectInfoResponse> service::project_info::Create(
    dto::CreateProjectInfoRequest request) {
  domain::ProjectInfo p;
  p.setName(std::move(request.name));
  if (request.developer) p.setDeveloper(std::move(*request.developer));
  if (request.scale) p.setScale(std::move(*request.scale));
  if (request.description) p.setDescription(std::move(*request.description));
  if (request.map_lat) p.setMapLat(*request.map_lat);
  if (request.map_lng) p.setMapLng(*request.map_lng);
  p.setMapZoom(request.map_zoom);

  auto created{co_await repo::project_info::Create(p)};
  co_return ToProjectInfoResponse(created);
}

drogon::Task<void> service::project_info::Update(
    std::string id, dto::UpdateProjectInfoRequest request) {
  auto existing_opt{co_await repo::project_info::FindById(id)};
  if (!existing_opt) {
    throw std::runtime_error{"Project info not found"};
  }

  auto existing{std::move(*existing_opt)};

  if (request.name) existing.setName(std::move(*request.name));
  if (request.developer) existing.setDeveloper(std::move(*request.developer));
  if (request.scale) existing.setScale(std::move(*request.scale));
  if (request.description) existing.setDescription(std::move(*request.description));
  if (request.map_lat) existing.setMapLat(*request.map_lat);
  if (request.map_lng) existing.setMapLng(*request.map_lng);
  if (request.map_zoom) existing.setMapZoom(*request.map_zoom);

  co_await repo::project_info::Update(existing);
}

drogon::Task<void> service::project_info::Delete(std::string id) {
  co_await repo::project_info::DeleteById(std::move(id));
}

drogon::Task<std::optional<dto::ProjectInfoResponse>> service::project_info::GetById(
    std::string id) {
  auto p_opt{co_await repo::project_info::FindById(std::move(id))};
  if (p_opt) {
    co_return ToProjectInfoResponse(std::move(*p_opt));
  }
  co_return std::nullopt;
}

drogon::Task<std::vector<dto::ProjectInfoResponse>> service::project_info::GetAll() {
  auto projects_raw{co_await repo::project_info::FindAll()};
  std::vector<dto::ProjectInfoResponse> result;
  result.reserve(projects_raw.size());
  
  std::ranges::transform(projects_raw, std::back_inserter(result), ToProjectInfoResponse);
  
  co_return result;
}
