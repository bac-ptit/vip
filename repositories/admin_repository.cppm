//
// Created by bac on 3/27/26.
//

module;
#include <drogon/orm/DbClient.h>
#include <drogon/HttpTypes.h>
#include <model.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

export module repo:admin;

import domain;

export namespace repo::admin {

using Admin = domain::Admin;

// Find operations
[[nodiscard]] drogon::Task<std::optional<Admin>> FindById(
    std::string_view id);

[[nodiscard]] drogon::Task<std::optional<Admin>> FindByUsername(
    std::string_view username);

[[nodiscard]] drogon::Task<std::vector<Admin>> FindAll();

[[nodiscard]] drogon::Task<std::vector<Admin>> FindActiveAdmins();

// Create operation - throws on error
[[nodiscard]] drogon::Task<Admin> Create(const Admin& admin);

// Update operation - throws on error
[[nodiscard]] drogon::Task<size_t> Update(const Admin& admin);

// Delete operations
[[nodiscard]] drogon::Task<size_t> DeleteById(std::string_view id);

[[nodiscard]] drogon::Task<bool> SoftDeleteById(std::string_view id);

// Check operations
[[nodiscard]] drogon::Task<bool> ExistsByUsername(std::string_view username);

[[nodiscard]] drogon::Task<bool> ExistsById(std::string_view id);

// Count operations
[[nodiscard]] drogon::Task<size_t> CountAll();

[[nodiscard]] drogon::Task<size_t> CountActive();

}  // namespace repo::admin
