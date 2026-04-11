#include <utility>
#include <stdexcept>
#include <vector>
#include <optional>
#include <string>
#include <ranges>
#include "registrations_service.h"

#include <drogon/HttpTypes.h>
#include <models/RegistrationInterests.h>
#include <models/Registrations.h>

#include <dto/dto.h>
#include <repositories/repo.h>
#include <domains/domain.h>

namespace service::registrations {

static dto::RegistrationResponse MapToResponse(
    const domain::Registrations& registration, const std::vector<domain::RegistrationInterests>& interests) {
  dto::RegistrationResponse response;
  response.id = registration.getId();
  response.full_name = registration.getFullName();
  response.phone = registration.getPhone();
  response.email = registration.getEmail();
  response.address = registration.getAddress();
  response.note = registration.getNote();
  response.is_contacted = registration.getIsContacted();
  response.contacted_by = registration.getContactedBy();
  response.contacted_at = registration.getContactedAt();
  response.created_at = registration.getCreatedAt();

  response.interests = std::make_shared<std::vector<std::string>>();
  for (const auto& interest : interests) {
    if (auto type{interest.getProductType()}; type) {
      response.interests->push_back(*type);
    }
  }

  return response;
}

drogon::Task<dto::RegistrationResponse> Create(
    dto::CreateRegistrationRequest request) {
  domain::Registrations registration;
  registration.setFullName(std::move(request.full_name));
  registration.setPhone(std::move(request.phone));
  if (request.email) {
    registration.setEmail(std::move(*request.email));
  }
  if (request.address) {
    registration.setAddress(std::move(*request.address));
  }
  if (request.note) {
    registration.setNote(std::move(*request.note));
  }

  auto created_registration{co_await repo::registrations::Create(std::move(registration))};

  std::vector<domain::RegistrationInterests> created_interests;
  for (const auto& type : request.interests) {
    domain::RegistrationInterests registration_interest;
    registration_interest.setRegistrationId(created_registration.getValueOfId());
    registration_interest.setProductType(type);
    auto ri_created{co_await repo::registration_interests::Create(std::move(registration_interest))};
    created_interests.push_back(std::move(ri_created));
  }

  co_return MapToResponse(created_registration, created_interests);
}

drogon::Task<std::optional<dto::RegistrationResponse>> GetById(
    std::string registration_id) {
  auto registration_opt{co_await repo::registrations::FindById(registration_id)};
  if (!registration_opt) {
    co_return std::nullopt;
  }
  auto registration{std::move(*registration_opt)};

  auto interests_raw{co_await repo::registration_interests::FindByRegistrationId(registration_id)};
  std::vector<domain::RegistrationInterests> interests;
  interests.reserve(interests_raw.size());
  std::ranges::transform(interests_raw, std::back_inserter(interests), [](auto& ri) {
    return std::move(ri);
  });
  
  co_return MapToResponse(registration, interests);
}

drogon::Task<std::vector<dto::RegistrationResponse>> GetAll() {
  auto registrations_raw{co_await repo::registrations::FindAll()};
  std::vector<dto::RegistrationResponse> responses;
  responses.reserve(registrations_raw.size());
  auto db_client{drogon::app().getDbClient()};
  for (auto& registration_raw : registrations_raw) {
    auto registration{std::move(registration_raw)};
    auto interests_raw{co_await repo::registration_interests::FindByRegistrationId(
        registration.getValueOfId())};
    std::vector<domain::RegistrationInterests> interests;
    interests.reserve(interests_raw.size());
    std::ranges::transform(interests_raw, std::back_inserter(interests), [](auto& ri) {
      return std::move(ri);
    });
    responses.push_back(MapToResponse(registration, interests));
  }

  co_return responses;
}

drogon::Task<void> Update(std::string registration_id,
                          dto::UpdateRegistrationRequest request) {
  auto registration_opt{co_await repo::registrations::FindById(registration_id)};
  if (!registration_opt) {
    throw std::runtime_error{"Registration not found"};
  }
  auto registration{std::move(*registration_opt)};

  if (request.is_contacted) {
    registration.setIsContacted(*request.is_contacted);
  }
  if (request.note) {
    registration.setNote(std::move(*request.note));
  }

  co_await repo::registrations::Update(registration);
}

drogon::Task<void> Delete(std::string registration_id) {
  co_await repo::registration_interests::DeleteByRegistrationId(registration_id);
  co_await repo::registrations::DeleteById(std::move(registration_id));
}

}  // namespace service::registrations
