#pragma once
#include <drogon/drogon.h>
#include <optional>
#include <string>
#include <vector>
#include "dto/dto.h"

namespace service::media {

[[nodiscard]] drogon::Task<dto::MediaResponse> Create(
    dto::CreateMediaRequest request, std::optional<std::string> admin_id);

[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateMediaRequest request);

[[nodiscard]] drogon::Task<void> Delete(std::string id);

[[nodiscard]] drogon::Task<std::optional<dto::MediaResponse>> GetById(
    std::string id);

[[nodiscard]] drogon::Task<std::vector<dto::MediaResponse>> GetAll();

[[nodiscard]] drogon::Task<std::vector<dto::MediaResponse>> GetActiveMedia();

}  // namespace service::media
