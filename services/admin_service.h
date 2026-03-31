#pragma once
#include <drogon/drogon.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include "dto/dto.h"

namespace service::admin {

[[nodiscard]] drogon::Task<dto::AdminResponse> Register(
    dto::RegisterAdminRequest request);

[[nodiscard]] drogon::Task<std::optional<dto::AdminResponse>> Login(
    dto::LoginRequest request);

[[nodiscard]] drogon::Task<void> UpdateProfile(
    std::string id, dto::UpdateProfileRequest request);

[[nodiscard]] drogon::Task<void> ChangePassword(
    std::string id, dto::ChangePasswordRequest request);

[[nodiscard]] drogon::Task<void> Deactivate(std::string id);

[[nodiscard]] drogon::Task<void> Activate(std::string id);

[[nodiscard]] drogon::Task<std::optional<dto::AdminResponse>> GetById(
    std::string id);

[[nodiscard]] drogon::Task<std::optional<dto::AdminResponse>> GetByUsername(
    std::string username);

[[nodiscard]] drogon::Task<std::vector<dto::AdminResponse>> GetAll();

[[nodiscard]] drogon::Task<std::vector<dto::AdminResponse>> GetActiveAdmins();

[[nodiscard]] drogon::Task<bool> IsUsernameAvailable(std::string username);

}  // namespace service::admin
