#pragma once
#include <vector>
#include <optional>
#include <string>

#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>

#include <dto/dto.h>

namespace service::admin_sessions {

[[nodiscard]] drogon::Task<std::optional<dto::AdminSessionResponse>> GetById(std::string session_id);
[[nodiscard]] drogon::Task<std::optional<dto::AdminSessionResponse>> GetByRefreshToken(std::string token);
[[nodiscard]] drogon::Task<std::vector<dto::AdminSessionResponse>> GetByAdminId(std::string admin_id);
[[nodiscard]] drogon::Task<void> RevokeSession(std::string session_id);
[[nodiscard]] drogon::Task<void> RevokeAllAdminSessions(std::string admin_id);

}  // namespace service::admin_sessions
