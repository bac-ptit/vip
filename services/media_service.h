#pragma once
#include <vector>
#include <optional>
#include <string>

#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>

#include <dto/dto.h>

namespace service::media {

/**
 * @brief Create media.
 */
[[nodiscard]] drogon::Task<dto::MediaResponse> Create(
    dto::CreateMediaRequest request, std::optional<std::string> admin_id);

/**
 * @brief Update media.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string media_id, dto::UpdateMediaRequest request);

/**
 * @brief Delete media.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string media_id);

/**
 * @brief Get media by ID.
 */
[[nodiscard]] drogon::Task<std::optional<dto::MediaResponse>> GetById(
    std::string media_id);

/**
 * @brief Get all media.
 */
[[nodiscard]] drogon::Task<std::vector<dto::MediaResponse>> GetAll();

/**
 * @brief Get active media.
 */
[[nodiscard]] drogon::Task<std::vector<dto::MediaResponse>> GetActiveMedia();

}  // namespace service::media
