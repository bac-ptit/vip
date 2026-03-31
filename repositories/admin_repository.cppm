//
// Created by bac on 3/27/26.
//

module;
#include <drogon/orm/DbClient.h>
#include <drogon/HttpTypes.h>
#include <model.h>
export module repo:admin;
import std;


import domain;

export namespace repo::admin {

using Admin = domain::Admin;

// Find operations
[[nodiscard]] drogon::Task<std::optional<Admin>> FindById(
    std::string id);

[[nodiscard]] drogon::Task<std::optional<Admin>> FindByUsername(
    std::string username);

[[nodiscard]] drogon::Task<std::vector<Admin>> FindAll();

[[nodiscard]] drogon::Task<std::vector<Admin>> FindActiveAdmins();

// Create operation - throws on error
[[nodiscard]] drogon::Task<Admin> Create(const Admin& admin);

// Update operation - throws on error
[[nodiscard]] drogon::Task<size_t> Update(const Admin& admin);

// Delete operations
[[nodiscard]] drogon::Task<size_t> DeleteById(std::string id);

[[nodiscard]] drogon::Task<bool> SoftDeleteById(std::string id);

// Check operations
[[nodiscard]] drogon::Task<bool> ExistsByUsername(std::string username);

[[nodiscard]] drogon::Task<bool> ExistsById(std::string id);

// Count operations
[[nodiscard]] drogon::Task<size_t> CountAll();

[[nodiscard]] drogon::Task<size_t> CountActive();

}  // namespace repo::admin
