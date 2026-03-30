//
// Created by bac on 3/27/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

export module service:registrations;

import dto;

export namespace service::registrations {

[[nodiscard]] drogon::Task<dto::RegistrationResponse> Create(
    dto::CreateRegistrationRequest request);

[[nodiscard]] drogon::Task<std::optional<dto::RegistrationResponse>> GetById(
    std::string id);

[[nodiscard]] drogon::Task<std::vector<dto::RegistrationResponse>> GetAll();

[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateRegistrationRequest request);

[[nodiscard]] drogon::Task<void> Delete(std::string id);

}  // namespace service::registrations
