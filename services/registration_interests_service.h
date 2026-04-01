#pragma once
#include <vector>
#include <string>
//
// Created by bac on 3/27/26.
//


#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>




#include <dto/dto.h>

namespace service::registration_interests {

[[nodiscard]] drogon::Task<dto::RegistrationInterestResponse> Create(
    dto::CreateRegistrationInterestRequest request);

[[nodiscard]] drogon::Task<std::vector<dto::RegistrationInterestResponse>>
GetByRegistrationId(std::string registration_id);

[[nodiscard]] drogon::Task<std::vector<dto::RegistrationInterestResponse>> GetAll();

[[nodiscard]] drogon::Task<void> Delete(std::string registrationId);

}  // namespace service::registration_interests
