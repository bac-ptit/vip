#pragma once
#include <drogon/drogon.h>
#include <optional>
#include <string>
#include <vector>
#include "dto/dto.h"

namespace service::floor_plans {

[[nodiscard]] drogon::Task<dto::FloorPlanResponse> Create(
    dto::CreateFloorPlanRequest request, std::optional<std::string> admin_id);

[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateFloorPlanRequest request);

[[nodiscard]] drogon::Task<void> Delete(std::string id);

[[nodiscard]] drogon::Task<std::optional<dto::FloorPlanResponse>> GetById(
    std::string id);

[[nodiscard]] drogon::Task<std::vector<dto::FloorPlanResponse>> GetAll();

[[nodiscard]] drogon::Task<std::vector<dto::FloorPlanResponse>> GetActive();

}  // namespace service::floor_plans
