//
// Created by bac on 3/28/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

export module service:floor_plans;

import dto;

export namespace service::floor_plans {

/**
 * @brief Create floor plan.
 */
[[nodiscard]] drogon::Task<dto::FloorPlanResponse> Create(
    dto::CreateFloorPlanRequest request, std::optional<std::string> admin_id);

/**
 * @brief Update floor plan.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateFloorPlanRequest request);

/**
 * @brief Delete floor plan.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string id);

/**
 * @brief Get floor plan by ID.
 */
[[nodiscard]] drogon::Task<std::optional<dto::FloorPlanResponse>> GetById(
    std::string id);

/**
 * @brief Get all floor plans.
 */
[[nodiscard]] drogon::Task<std::vector<dto::FloorPlanResponse>> GetAll();

/**
 * @brief Get active floor plans.
 */
[[nodiscard]] drogon::Task<std::vector<dto::FloorPlanResponse>> GetActive();

}  // namespace service::floor_plans
