#pragma once
#include <vector>
#include <optional>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::registrations {

[[nodiscard]] drogon::Task<domain::Registrations> Create(domain::Registrations registration);
[[nodiscard]] drogon::Task<std::optional<domain::Registrations>> FindById(std::string registration_id);
[[nodiscard]] drogon::Task<std::vector<domain::Registrations>> FindAll();
[[nodiscard]] drogon::Task<size_t> Update(domain::Registrations registration);
[[nodiscard]] drogon::Task<bool> DeleteById(std::string registration_id);

}  // namespace repo::registrations
