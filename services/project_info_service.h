#pragma once
#include <vector>
#include <optional>
#include <string>
//
// Created by bac on 3/28/26.
//


#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>




#include <dto/dto.h>

namespace service::project_info {

/**
 * @brief Create project info.
 */
[[nodiscard]] drogon::Task<dto::ProjectInfoResponse> Create(
    dto::CreateProjectInfoRequest request);

/**
 * @brief Update project info.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string identifier, dto::UpdateProjectInfoRequest request);

/**
 * @brief Delete project info.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string identifier);

/**
 * @brief Get project info by ID.
 */
[[nodiscard]] drogon::Task<std::optional<dto::ProjectInfoResponse>> GetById(
    std::string identifier);

/**
 * @brief Get all project info.
 */
[[nodiscard]] drogon::Task<std::vector<dto::ProjectInfoResponse>> GetAll();

}  // namespace service::project_info
