#pragma once
#include <vector>
#include <optional>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::project_info {

[[nodiscard]] drogon::Task<domain::ProjectInfo> Create(domain::ProjectInfo project_info);
[[nodiscard]] drogon::Task<std::optional<domain::ProjectInfo>> FindById(std::string identifier);
[[nodiscard]] drogon::Task<std::vector<domain::ProjectInfo>> FindAll();
[[nodiscard]] drogon::Task<size_t> Update(domain::ProjectInfo project_info);
[[nodiscard]] drogon::Task<bool> DeleteById(std::string identifier);

}  // namespace repo::project_info
