#pragma once
#include <vector>
#include <optional>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::media {

[[nodiscard]] drogon::Task<domain::Media> Create(const domain::Media& m);
[[nodiscard]] drogon::Task<std::optional<domain::Media>> FindById(std::string mediaId);
[[nodiscard]] drogon::Task<std::vector<domain::Media>> FindAll();
[[nodiscard]] drogon::Task<std::vector<domain::Media>> FindActive();
[[nodiscard]] drogon::Task<void> Update(const domain::Media& m);
[[nodiscard]] drogon::Task<bool> DeleteById(std::string mediaId);

}  // namespace repo::media
