#pragma once
#include <vector>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::registration_interests {

[[nodiscard]] drogon::Task<domain::RegistrationInterests> Create(domain::RegistrationInterests ri);
[[nodiscard]] drogon::Task<std::vector<domain::RegistrationInterests>> FindByRegistrationId(std::string registration_id);
[[nodiscard]] drogon::Task<void> DeleteByRegistrationId(std::string registration_id);
[[nodiscard]] drogon::Task<std::vector<domain::RegistrationInterests>> FindAll();
[[nodiscard]] drogon::Task<bool> DeleteById(std::string registrationId);

}  // namespace repo::registration_interests
