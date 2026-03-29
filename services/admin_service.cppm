//
// Created by bac on 3/27/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

export module service:admin;

import dto;

export namespace service::admin {

/**
 * @brief Register a new admin with validation
 * @param request Registration data (passed by value for move optimization)
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
    dto::LoginRequest&& request);

/**
 * @brief Update admin profile
 * @param id Admin ID
 * @param request Update data (passed by value for move optimization)
 * @throws std::runtime_error if admin not found or validation fails
 */
[[nodiscard]] drogon::Task<void> UpdateProfile(
    std::string_view id, dto::UpdateProfileRequest request);

/**
 * @brief Change admin password
 * @param id Admin ID
 * @param request Password change data (passed by value for move optimization)
 * @throws std::runtime_error if admin not found or old password incorrect
 */
[[nodiscard]] drogon::Task<void> ChangePassword(
    std::string_view id, dto::ChangePasswordRequest request);

/**
 * @brief Deactivate admin account (soft delete)
 * @param id Admin ID
 * @throws std::runtime_error if admin not found
 */
[[nodiscard]] drogon::Task<void> Deactivate(std::string_view id);

/**
 * @brief Activate admin account
 * @param id Admin ID
 * @throws std::runtime_error if admin not found
 */
[[nodiscard]] drogon::Task<void> Activate(std::string_view id);

/**
 * @brief Get admin by ID
 * @param id Admin ID
 * @return Admin response if found, nullopt otherwise
 */
[[nodiscard]] drogon::Task<std::optional<dto::AdminResponse>> GetById(
    std::string_view id);

/**
 * @brief Get admin by username
 * @param username Admin username
 * @return Admin response if found, nullopt otherwise
 */
[[nodiscard]] drogon::Task<std::optional<dto::AdminResponse>> GetByUsername(
    std::string_view username);

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
    std::string_view username);

}  // namespace service::admin
