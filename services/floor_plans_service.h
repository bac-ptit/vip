#pragma once
#include <vector>
#include <optional>
#include <string>

#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>

#include <dto/dto.h>

namespace service::floor_plans {

/**
 * @brief Create floor plan.
 */
[[nodiscard]] drogon::Task<dto::FloorPlanResponse> Create(
    dto::CreateFloorPlanRequest request, std::optional<std::string> admin_id);

/**
 * @brief Update floor plan.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string floor_plan_id, dto::UpdateFloorPlanRequest request);

/**
 * @brief Delete floor plan.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string floor_plan_id);

/**
 * @brief Get floor plan by ID.
 */
[[nodiscard]] drogon::Task<std::optional<dto::FloorPlanResponse>> GetById(
    std::string floor_plan_id);

/**
 * @brief Get all floor plans.
 */
[[nodiscard]] drogon::Task<std::vector<dto::FloorPlanResponse>> GetAll();

/**
 * @brief Get active floor plans.
 */
[[nodiscard]] drogon::Task<std::vector<dto::FloorPlanResponse>> GetActive();

}  // namespace service::floor_plans
