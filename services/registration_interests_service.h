#pragma once
#include <drogon/drogon.h>
#include <string>
#include <vector>
#include "dto/dto.h"

namespace service::registration_interests {

[[nodiscard]] drogon::Task<dto::RegistrationInterestResponse> Create(
    dto::CreateRegistrationInterestRequest request);

[[nodiscard]] drogon::Task<std::vector<dto::RegistrationInterestResponse>>
GetByRegistrationId(std::string registration_id);

[[nodiscard]] drogon::Task<std::vector<dto::RegistrationInterestResponse>> GetAll();

[[nodiscard]] drogon::Task<void> Delete(std::string id);

}  // namespace service::registration_interests
