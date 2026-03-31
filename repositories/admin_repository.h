#pragma once
#include <drogon/orm/DbClient.h>
#include <drogon/HttpTypes.h>
#include <model.h>
#include <optional>
#include <string>
#include <vector>
#include "domains/domain.h"

namespace repo::admin {

using Admin = domain::Admin;

[[nodiscard]] drogon::Task<std::optional<Admin>> FindById(std::string id);

[[nodiscard]] drogon::Task<std::optional<Admin>> FindByUsername(
    std::string username);

[[nodiscard]] drogon::Task<std::vector<Admin>> FindAll();

[[nodiscard]] drogon::Task<std::vector<Admin>> FindActiveAdmins();

[[nodiscard]] drogon::Task<Admin> Create(const Admin& admin);

[[nodiscard]] drogon::Task<size_t> Update(const Admin& admin);

[[nodiscard]] drogon::Task<size_t> DeleteById(std::string id);

[[nodiscard]] drogon::Task<bool> SoftDeleteById(std::string id);

[[nodiscard]] drogon::Task<bool> ExistsByUsername(std::string username);

[[nodiscard]] drogon::Task<bool> ExistsById(std::string id);

[[nodiscard]] drogon::Task<size_t> CountAll();

[[nodiscard]] drogon::Task<size_t> CountActive();

}  // namespace repo::admin
