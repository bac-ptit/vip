#include "floor_plans_service.h"
#include <repositories/floor_plans_repository.h>
#include <domains/floor_plans.h>
#include <algorithm>
#include <ranges>

namespace service::floor_plans {

static dto::FloorPlanResponse ToFloorPlanResponse(const domain::FloorPlans& f) {
  return {
      .id = f.getId(),
      .title = f.getTitle(),
      .url = f.getUrl(),
      .sort_order = f.getSortOrder(),
      .is_active = f.getIsActive(),
      .uploaded_by = f.getUploadedBy(),
      .created_at = f.getCreatedAt()};
}

drogon::Task<dto::FloorPlanResponse> Create(
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

drogon::Task<void> Update(
    std::string floor_plan_id, dto::UpdateFloorPlanRequest request) {
  auto existing_opt{co_await repo::floor_plans::FindById(floor_plan_id)};
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

drogon::Task<void> Delete(std::string floor_plan_id) {
  co_await repo::floor_plans::DeleteById(std::move(floor_plan_id));
}

drogon::Task<std::optional<dto::FloorPlanResponse>> GetById(
    std::string floor_plan_id) {
  auto f_opt{co_await repo::floor_plans::FindById(floor_plan_id)};
  if (f_opt) {
    co_return ToFloorPlanResponse(std::move(*f_opt));
  }
  co_return std::nullopt;
}

drogon::Task<std::vector<dto::FloorPlanResponse>> GetAll() {
  auto floor_plans_raw{co_await repo::floor_plans::FindAll()};
  std::vector<dto::FloorPlanResponse> result;
  result.reserve(floor_plans_raw.size());
  
  std::ranges::transform(floor_plans_raw, std::back_inserter(result), ToFloorPlanResponse);
  
  co_return result;
}

drogon::Task<std::vector<dto::FloorPlanResponse>> GetActive() {
  auto floor_plans_raw{co_await repo::floor_plans::FindActive()};
  std::vector<dto::FloorPlanResponse> result;
  result.reserve(floor_plans_raw.size());
  
  std::ranges::transform(floor_plans_raw, std::back_inserter(result), ToFloorPlanResponse);
  
  co_return result;
}

}  // namespace service::floor_plans
