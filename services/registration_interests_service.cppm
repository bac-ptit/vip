//
// Created by bac on 3/27/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
export module service:registration_interests;
import std;


import dto;

export namespace service::registration_interests {

[[nodiscard]] drogon::Task<dto::RegistrationInterestResponse> Create(
    dto::CreateRegistrationInterestRequest request);

[[nodiscard]] drogon::Task<std::vector<dto::RegistrationInterestResponse>>
GetByRegistrationId(std::string registration_id);

[[nodiscard]] drogon::Task<std::vector<dto::RegistrationInterestResponse>> GetAll();

[[nodiscard]] drogon::Task<void> Delete(std::string id);

}  // namespace service::registration_interests
