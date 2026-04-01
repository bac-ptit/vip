#include <utility>
#include <stdexcept>
#include <vector>
#include <optional>
#include <string>
#include <ranges>
#include "project_info_service.h"

#include <drogon/drogon.h>
#include <models/ProjectInfo.h>

#include <repositories/repo.h>
#include <dto/dto.h>
#include <domains/domain.h>

using namespace drogon;

namespace service::project_info {

static dto::ProjectInfoResponse ToProjectInfoResponse(const domain::ProjectInfo& project_info) {
  dto::ProjectInfoResponse response;
  response.id = project_info.getId();
  response.name = project_info.getName();
  response.developer = project_info.getDeveloper();
  response.scale = project_info.getScale();
  response.description = project_info.getDescription();
  response.map_lat = project_info.getMapLat();
  response.map_lng = project_info.getMapLng();
  response.map_zoom = project_info.getMapZoom();
  response.updated_by = project_info.getUpdatedBy();
  response.updated_at = project_info.getUpdatedAt();
  return response;
}

drogon::Task<dto::ProjectInfoResponse> Create(
    dto::CreateProjectInfoRequest request) {
  domain::ProjectInfo project_info;
  project_info.setName(std::move(request.name));
  if (request.developer) project_info.setDeveloper(std::move(*request.developer));
  if (request.scale) project_info.setScale(std::move(*request.scale));
  if (request.description) project_info.setDescription(std::move(*request.description));
  if (request.map_lat) project_info.setMapLat(*request.map_lat);
  if (request.map_lng) project_info.setMapLng(*request.map_lng);
  project_info.setMapZoom(request.map_zoom);

  auto created{co_await repo::project_info::Create(project_info)};
  co_return ToProjectInfoResponse(created);
}

drogon::Task<void> Update(
    std::string identifier, dto::UpdateProjectInfoRequest request) {
  auto project_opt{co_await repo::project_info::FindById(identifier)};
  if (!project_opt) {
    throw std::runtime_error{"Project info not found"};
  }

  auto existing{std::move(*project_opt)};

  if (request.name) existing.setName(std::move(*request.name));
  if (request.developer) existing.setDeveloper(std::move(*request.developer));
  if (request.scale) existing.setScale(std::move(*request.scale));
  if (request.description) existing.setDescription(std::move(*request.description));
  if (request.map_lat) existing.setMapLat(*request.map_lat);
  if (request.map_lng) existing.setMapLng(*request.map_lng);
  if (request.map_zoom) existing.setMapZoom(*request.map_zoom);

  co_await repo::project_info::Update(existing);
}

drogon::Task<void> Delete(std::string identifier) {
  co_await repo::project_info::DeleteById(std::move(identifier));
}

drogon::Task<std::optional<dto::ProjectInfoResponse>> GetById(
    std::string identifier) {
  auto project_opt{co_await repo::project_info::FindById(std::move(identifier))};
  if (project_opt) {
    co_return ToProjectInfoResponse(std::move(*project_opt));
  }
  co_return std::nullopt;
}

drogon::Task<std::vector<dto::ProjectInfoResponse>> GetAll() {
  auto projects_raw{co_await repo::project_info::FindAll()};
  std::vector<dto::ProjectInfoResponse> result;
  result.reserve(projects_raw.size());
  
  std::ranges::transform(projects_raw, std::back_inserter(result), ToProjectInfoResponse);
  
  co_return result;
}

}  // namespace service::project_info
