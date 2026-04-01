#pragma once
#include <vector>
#include <optional>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::admin_sessions {

[[nodiscard]] drogon::Task<std::optional<domain::AdminSessions>> FindById(std::string session_id);
[[nodiscard]] drogon::Task<std::optional<domain::AdminSessions>> FindByRefreshToken(std::string token);
[[nodiscard]] drogon::Task<std::vector<domain::AdminSessions>> FindByAdminId(std::string admin_id);
[[nodiscard]] drogon::Task<void> Create(domain::AdminSessions session);
[[nodiscard]] drogon::Task<void> Update(domain::AdminSessions session);
[[nodiscard]] drogon::Task<void> DeleteById(std::string session_id);
[[nodiscard]] drogon::Task<void> DeleteByAdminId(std::string admin_id);

}  // namespace repo::admin_sessions
