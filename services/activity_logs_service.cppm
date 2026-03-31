//
// Created by bac on 3/29/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
export module service:activity_logs;
import std;


import dto;

export namespace service::activity_logs {

[[nodiscard]] drogon::Task<std::vector<dto::ActivityLogResponse>> GetAll();
[[nodiscard]] drogon::Task<std::vector<dto::ActivityLogResponse>> GetByAdminId(std::string adminId);
[[nodiscard]] drogon::Task<void> LogAction(std::string adminId, std::string action, std::string entity, std::string entityId, std::string detail, std::string ipAddress);

}  // namespace service::activity_logs
