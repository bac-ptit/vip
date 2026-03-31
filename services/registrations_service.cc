//
// Created by bac on 3/27/26.
//

#include <drogon/HttpTypes.h>
#include <models/RegistrationInterests.h>
#include <models/Registrations.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include "services/registrations_service.h"
#include "dto/dto.h"
#include "repositories/repo.h"
#include "domains/domain.h"

namespace service::registrations {

static dto::RegistrationResponse MapToResponse(
    const domain::Registrations& r, const std::vector<domain::RegistrationInterests>& interests) {
  dto::RegistrationResponse response;
  response.id = r.getId();
  response.full_name = r.getFullName();
  response.phone = r.getPhone();
  response.email = r.getEmail();
  response.address = r.getAddress();
  response.note = r.getNote();
  response.is_contacted = r.getIsContacted();
  response.contacted_by = r.getContactedBy();
  response.contacted_at = r.getContactedAt();
  response.created_at = r.getCreatedAt();

  response.interests = std::make_shared<std::vector<std::string>>();
  for (const auto& interest : interests) {
    if (auto type = interest.getProductType(); type) {
      response.interests->push_back(*type);
    }
  }

  return response;
}

drogon::Task<dto::RegistrationResponse> Create(
    dto::CreateRegistrationRequest request) {
  domain::Registrations r;
  r.setFullName(std::move(request.full_name));
  r.setPhone(std::move(request.phone));
  if (request.email) {
    r.setEmail(std::move(*request.email));
  }
  if (request.address) {
    r.setAddress(std::move(*request.address));
  }
  if (request.note) {
    r.setNote(std::move(*request.note));
  }

  auto created_r{co_await repo::registrations::Create(std::move(r))};

  std::vector<domain::RegistrationInterests> created_interests;
  for (const auto& type : request.interests) {
    domain::RegistrationInterests ri;
    ri.setRegistrationId(created_r.getValueOfId());
    ri.setProductType(type);
    auto ri_created{co_await repo::registration_interests::Create(std::move(ri))};
    created_interests.push_back(std::move(ri_created));
  }

  co_return MapToResponse(created_r, created_interests);
}

drogon::Task<std::optional<dto::RegistrationResponse>> GetById(
    std::string id) {
  auto r_opt{co_await repo::registrations::FindById(id)};
  if (!r_opt) {
    co_return std::nullopt;
  }
  auto r{std::move(*r_opt)};

  auto interests_raw{co_await repo::registration_interests::FindByRegistrationId(id)};
  std::vector<domain::RegistrationInterests> interests;
  interests.reserve(interests_raw.size());
  std::ranges::transform(interests_raw, std::back_inserter(interests), [](auto&& ri) {
    return std::move(ri);
  });
  
  co_return MapToResponse(r, interests);
}

drogon::Task<std::vector<dto::RegistrationResponse>> GetAll() {
  auto registrations_raw{co_await repo::registrations::FindAll()};
  std::vector<dto::RegistrationResponse> responses;
  responses.reserve(registrations_raw.size());

  for (auto&& r_raw : registrations_raw) {
    auto r{std::move(r_raw)};
    auto interests_raw{co_await repo::registration_interests::FindByRegistrationId(
        r.getValueOfId())};
    std::vector<domain::RegistrationInterests> interests;
    interests.reserve(interests_raw.size());
    std::ranges::transform(interests_raw, std::back_inserter(interests), [](auto&& ri) {
      return std::move(ri);
    });
    responses.push_back(MapToResponse(r, interests));
  }

  co_return responses;
}

drogon::Task<void> Update(std::string id,
                          dto::UpdateRegistrationRequest request) {
  auto r_opt{co_await repo::registrations::FindById(id)};
  if (!r_opt) {
    throw std::runtime_error{"Registration not found"};
  }
  auto r{std::move(*r_opt)};

  if (request.is_contacted) {
    r.setIsContacted(*request.is_contacted);
  }
  if (request.note) {
    r.setNote(std::move(*request.note));
  }

  co_await repo::registrations::Update(r);
}

drogon::Task<void> Delete(std::string id) {
  co_await repo::registration_interests::DeleteByRegistrationId(id);
  co_await repo::registrations::DeleteById(std::move(id));
}

}  // namespace service::registrations
