#pragma once
#include <vector>
#include <optional>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::floor_plans {

[[nodiscard]] drogon::Task<domain::FloorPlans> Create(const domain::FloorPlans& f);
[[nodiscard]] drogon::Task<std::optional<domain::FloorPlans>> FindById(std::string floorPlanId);
[[nodiscard]] drogon::Task<std::vector<domain::FloorPlans>> FindAll();
[[nodiscard]] drogon::Task<std::vector<domain::FloorPlans>> FindActive();
[[nodiscard]] drogon::Task<size_t> Update(const domain::FloorPlans& f);
[[nodiscard]] drogon::Task<bool> DeleteById(std::string floorPlanId);

}  // namespace repo::floor_plans
