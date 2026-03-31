//
// Created by bac on 3/29/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
export module service:admin_sessions;
import std;


import dto;

export namespace service::admin_sessions {

[[nodiscard]] drogon::Task<std::optional<dto::AdminSessionResponse>> GetById(std::string id);
[[nodiscard]] drogon::Task<std::optional<dto::AdminSessionResponse>> GetByRefreshToken(std::string token);
[[nodiscard]] drogon::Task<std::vector<dto::AdminSessionResponse>> GetByAdminId(std::string adminId);
[[nodiscard]] drogon::Task<void> RevokeSession(std::string id);
[[nodiscard]] drogon::Task<void> RevokeAllAdminSessions(std::string adminId);

}  // namespace service::admin_sessions
