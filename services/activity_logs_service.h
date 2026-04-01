#pragma once
#include <vector>
#include <string>
//
// Created by bac on 3/29/26.
//


#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>




#include <dto/dto.h>

namespace service::activity_logs {

[[nodiscard]] drogon::Task<std::vector<dto::ActivityLogResponse>> GetAll();
[[nodiscard]] drogon::Task<std::vector<dto::ActivityLogResponse>> GetByAdminId(std::string adminId);
[[nodiscard]] drogon::Task<void> LogAction(std::string adminId, std::string action, std::string entity, std::string entityId, std::string detail, std::string ipAddress);

}  // namespace service::activity_logs
