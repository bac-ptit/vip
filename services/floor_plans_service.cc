//
// Created by bac on 3/28/26.
//
#include <drogon/drogon.h>
#include <models/FloorPlans.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include "services/floor_plans_service.h"
#include "repositories/repo.h"
#include "dto/dto.h"
#include "domains/domain.h"

using namespace drogon;

namespace {
dto::FloorPlanResponse ToFloorPlanResponse(const domain::FloorPlans& f) {
  dto::FloorPlanResponse resp;
  resp.id = f.getId();
  resp.title = f.getTitle();
  resp.url = f.getUrl();
  resp.sort_order = f.getSortOrder();
  resp.is_active = f.getIsActive();
  resp.uploaded_by = f.getUploadedBy();
  resp.created_at = f.getCreatedAt();
  return resp;
}
}  // namespace

drogon::Task<dto::FloorPlanResponse> service::floor_plans::Create(
    dto::CreateFloorPlanRequest request, std::optional<std::string> admin_id) {
  domain::FloorPlans f;
  f.setTitle(std::move(request.title));
  f.setUrl(std::move(request.url));
  f.setSortOrder(request.sort_order);
  f.setIsActive(request.is_active);
  if (admin_id) f.setUploadedBy(*admin_id);

  auto created{co_await repo::floor_plans::Create(std::move(f))};
  co_return ToFloorPlanResponse(created);
}

drogon::Task<void> service::floor_plans::Update(
    std::string id, dto::UpdateFloorPlanRequest request) {
  auto existing_opt{co_await repo::floor_plans::FindById(id)};
  if (!existing_opt) {
    throw std::runtime_error{"Floor plan not found"};
  }

  auto existing{std::move(*existing_opt)};

  if (request.title) existing.setTitle(std::move(*request.title));
  if (request.url) existing.setUrl(std::move(*request.url));
  if (request.sort_order) existing.setSortOrder(*request.sort_order);
  if (request.is_active) existing.setIsActive(*request.is_active);

  co_await repo::floor_plans::Update(existing);
}

drogon::Task<void> service::floor_plans::Delete(std::string id) {
  co_await repo::floor_plans::DeleteById(std::move(id));
}

drogon::Task<std::optional<dto::FloorPlanResponse>> service::floor_plans::GetById(
    std::string id) {
  auto f_opt{co_await repo::floor_plans::FindById(std::move(id))};
  if (f_opt) {
    co_return ToFloorPlanResponse(std::move(*f_opt));
  }
  co_return std::nullopt;
}

drogon::Task<std::vector<dto::FloorPlanResponse>> service::floor_plans::GetAll() {
  auto floor_plans_raw{co_await repo::floor_plans::FindAll()};
  std::vector<dto::FloorPlanResponse> result;
  result.reserve(floor_plans_raw.size());
  
  std::ranges::transform(floor_plans_raw, std::back_inserter(result), ToFloorPlanResponse);
  
  co_return result;
}

drogon::Task<std::vector<dto::FloorPlanResponse>> service::floor_plans::GetActive() {
  auto floor_plans_raw{co_await repo::floor_plans::FindActive()};
  std::vector<dto::FloorPlanResponse> result;
  result.reserve(floor_plans_raw.size());
  
  std::ranges::transform(floor_plans_raw, std::back_inserter(result), ToFloorPlanResponse);
  
  co_return result;
}
