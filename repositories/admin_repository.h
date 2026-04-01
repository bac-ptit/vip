#pragma once
#include <vector>
#include <optional>
#include <string>
#include <drogon/HttpTypes.h>
#include <drogon/orm/DbClient.h>
#include <domains/domain.h>

namespace repo::admin {

using Admin = domain::Admin;

// Find operations
[[nodiscard]] drogon::Task<std::optional<Admin>> FindById(std::string admin_id);
[[nodiscard]] drogon::Task<std::optional<Admin>> FindByUsername(std::string username);
[[nodiscard]] drogon::Task<std::vector<Admin>> FindAll();
[[nodiscard]] drogon::Task<std::vector<Admin>> FindActiveAdmins();

// Create/Update
[[nodiscard]] drogon::Task<Admin> Create(const Admin& admin);
[[nodiscard]] drogon::Task<size_t> Update(const Admin& admin);

// Delete
[[nodiscard]] drogon::Task<size_t> DeleteById(std::string admin_id);
[[nodiscard]] drogon::Task<bool> SoftDeleteById(std::string admin_id);

// Check
[[nodiscard]] drogon::Task<bool> ExistsByUsername(std::string username);
[[nodiscard]] drogon::Task<bool> ExistsById(std::string admin_id);

// Count
[[nodiscard]] drogon::Task<size_t> CountAll();
[[nodiscard]] drogon::Task<size_t> CountActive();

}  // namespace repo::admin
