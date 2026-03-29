//
// Created by bac on 3/29/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
#include <optional>
#include <string>
#include <vector>

export module service:admin_sessions;

import dto;

export namespace service::admin_sessions {

[[nodiscard]] drogon::Task<std::optional<dto::AdminSessionResponse>> GetById(const std::string& id);
[[nodiscard]] drogon::Task<std::optional<dto::AdminSessionResponse>> GetByRefreshToken(const std::string& token);
[[nodiscard]] drogon::Task<std::vector<dto::AdminSessionResponse>> GetByAdminId(const std::string& adminId);
[[nodiscard]] drogon::Task<void> RevokeSession(const std::string& id);
[[nodiscard]] drogon::Task<void> RevokeAllAdminSessions(const std::string& adminId);

}  // namespace service::admin_sessions
