#pragma once
#include <vector>
#include <optional>
#include <string>

#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>

#include <dto/dto.h>

namespace service::registrations {

[[nodiscard]] drogon::Task<dto::RegistrationResponse> Create(
    dto::CreateRegistrationRequest request);

[[nodiscard]] drogon::Task<std::optional<dto::RegistrationResponse>> GetById(
    std::string registration_id);

[[nodiscard]] drogon::Task<std::vector<dto::RegistrationResponse>> GetAll();

[[nodiscard]] drogon::Task<void> Update(
    std::string registration_id, dto::UpdateRegistrationRequest request);

[[nodiscard]] drogon::Task<void> Delete(std::string registration_id);

}  // namespace service::registrations
