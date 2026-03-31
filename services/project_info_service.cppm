//
// Created by bac on 3/28/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
export module service:project_info;
import std;


import dto;

export namespace service::project_info {

/**
 * @brief Create project info.
 */
[[nodiscard]] drogon::Task<dto::ProjectInfoResponse> Create(
    dto::CreateProjectInfoRequest request);

/**
 * @brief Update project info.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateProjectInfoRequest request);

/**
 * @brief Delete project info.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string id);

/**
 * @brief Get project info by ID.
 */
[[nodiscard]] drogon::Task<std::optional<dto::ProjectInfoResponse>> GetById(
    std::string id);

/**
 * @brief Get all project info.
 */
[[nodiscard]] drogon::Task<std::vector<dto::ProjectInfoResponse>> GetAll();

}  // namespace service::project_info
