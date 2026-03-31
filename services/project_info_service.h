#pragma once
#include <drogon/drogon.h>
#include <optional>
#include <string>
#include <vector>
#include "dto/dto.h"

namespace service::project_info {

[[nodiscard]] drogon::Task<dto::ProjectInfoResponse> Create(
    dto::CreateProjectInfoRequest request);

[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateProjectInfoRequest request);

[[nodiscard]] drogon::Task<void> Delete(std::string id);

[[nodiscard]] drogon::Task<std::optional<dto::ProjectInfoResponse>> GetById(
    std::string id);

[[nodiscard]] drogon::Task<std::vector<dto::ProjectInfoResponse>> GetAll();

}  // namespace service::project_info
