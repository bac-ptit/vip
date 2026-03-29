//
// Created by bac on 3/29/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
#include <optional>
#include <string>
#include <vector>

export module service:activity_logs;

import dto;

export namespace service::activity_logs {

[[nodiscard]] drogon::Task<std::vector<dto::ActivityLogResponse>> GetAll();
[[nodiscard]] drogon::Task<std::vector<dto::ActivityLogResponse>> GetByAdminId(const std::string& adminId);
[[nodiscard]] drogon::Task<void> LogAction(const std::string& adminId, const std::string& action, const std::string& entity, const std::string& entityId, const std::string& detail, const std::string& ipAddress);

}  // namespace service::activity_logs
