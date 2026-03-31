//
// Created by bac on 3/28/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
export module service:media;
import std;


import dto;

export namespace service::media {

/**
 * @brief Create media.
 */
[[nodiscard]] drogon::Task<dto::MediaResponse> Create(
    dto::CreateMediaRequest request, std::optional<std::string> admin_id);

/**
 * @brief Update media.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateMediaRequest request);

/**
 * @brief Delete media.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string id);

/**
 * @brief Get media by ID.
 */
[[nodiscard]] drogon::Task<std::optional<dto::MediaResponse>> GetById(
    std::string id);

/**
 * @brief Get all media.
 */
[[nodiscard]] drogon::Task<std::vector<dto::MediaResponse>> GetAll();

/**
 * @brief Get active media.
 */
[[nodiscard]] drogon::Task<std::vector<dto::MediaResponse>> GetActiveMedia();

}  // namespace service::media
