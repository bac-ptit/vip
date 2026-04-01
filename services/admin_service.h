#pragma once
#include <vector>
#include <optional>
#include <string>

#include <drogon/drogon.h>

#include <dto/dto.h>

namespace service::admin {

/**
 * @brief Register a new admin with validation
 * @param request Registration data
 * @return Created admin response
 * @throws std::runtime_error if validation fails or username exists
 */
[[nodiscard]] drogon::Task<dto::AdminResponse> Register(
    dto::RegisterAdminRequest request);

/**
 * @brief Authenticate admin with username and password
 * @param request Login credentials
 * @return Admin response if authenticated, nullopt if credentials invalid
 * @throws std::runtime_error if account is deactivated
 */
[[nodiscard]] drogon::Task<std::optional<dto::AdminResponse>> Login(
    dto::LoginRequest request);

/**
 * @brief Update admin profile
 * @param admin_id Admin ID
 * @param request Update data
 * @throws std::runtime_error if admin not found or validation fails
 */
[[nodiscard]] drogon::Task<void> UpdateProfile(
    std::string admin_id, dto::UpdateProfileRequest request);

/**
 * @brief Change admin password
 * @param admin_id Admin ID
 * @param request Password change data
 * @throws std::runtime_error if admin not found or old password incorrect
 */
[[nodiscard]] drogon::Task<void> ChangePassword(
    std::string admin_id, dto::ChangePasswordRequest request);

/**
 * @brief Deactivate admin account (soft delete)
 * @param admin_id Admin ID
 * @throws std::runtime_error if admin not found
 */
[[nodiscard]] drogon::Task<void> Deactivate(std::string admin_id);

/**
 * @brief Activate admin account
 * @param admin_id Admin ID
 * @throws std::runtime_error if admin not found
 */
[[nodiscard]] drogon::Task<void> Activate(std::string admin_id);

/**
 * @brief Get admin by ID
 * @param admin_id Admin ID
 * @return Admin response if found, nullopt otherwise
 */
[[nodiscard]] drogon::Task<std::optional<dto::AdminResponse>> GetById(
    std::string admin_id);

/**
 * @brief Get admin by username
 * @param username Admin username
 * @return Admin response if found, nullopt otherwise
 */
[[nodiscard]] drogon::Task<std::optional<dto::AdminResponse>> GetByUsername(
    std::string username);

/**
 * @brief Get all admins
 * @return Vector of admin responses
 */
[[nodiscard]] drogon::Task<std::vector<dto::AdminResponse>> GetAll();

/**
 * @brief Get only active admins
 * @return Vector of active admin responses
 */
[[nodiscard]] drogon::Task<std::vector<dto::AdminResponse>> GetActiveAdmins();

/**
 * @brief Check if username is available
 * @param username Username to check
 * @return true if available, false if taken
 */
[[nodiscard]] drogon::Task<bool> IsUsernameAvailable(
    std::string username);

}  // namespace service::admin
