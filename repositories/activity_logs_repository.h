#pragma once
#include <vector>
#include <optional>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::activity_logs {

[[nodiscard]] drogon::Task<std::optional<domain::ActivityLogs>> FindById(std::string identifier);
[[nodiscard]] drogon::Task<std::vector<domain::ActivityLogs>> FindByAdminId(std::string admin_id);
[[nodiscard]] drogon::Task<std::vector<domain::ActivityLogs>> FindAll();
[[nodiscard]] drogon::Task<void> Create(domain::ActivityLogs log);

}  // namespace repo::activity_logs
