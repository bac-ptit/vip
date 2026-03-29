//
// Created by bac on 3/27/26.
//

module;
#include <drogon/HttpTypes.h>
#include <models/RegistrationInterests.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

module service;

import dto;
import repo;
import domain;

namespace service::registration_interests {

static dto::RegistrationInterestResponse MapToResponse(
    const domain::RegistrationInterests& ri) {
  dto::RegistrationInterestResponse response;
  response.id = ri.getId();
  response.registration_id = ri.getRegistrationId();
  response.product_type = ri.getProductType();
  return response;
}

drogon::Task<dto::RegistrationInterestResponse> Create(
    dto::CreateRegistrationInterestRequest request) {
  domain::RegistrationInterests ri;
  ri.setRegistrationId(request.registration_id);
  ri.setProductType(request.product_type);

  auto created_ri{co_await repo::registration_interests::Create(ri)};
  co_return MapToResponse(created_ri);
}

drogon::Task<std::vector<dto::RegistrationInterestResponse>>
GetByRegistrationId(std::string_view registration_id) {
  auto interests{
      co_await repo::registration_interests::FindByRegistrationId(registration_id)};
  std::vector<dto::RegistrationInterestResponse> responses;
  responses.reserve(interests.size());
  for (auto&& ri : interests) {
    responses.push_back(MapToResponse(ri));
  }
  co_return responses;
}

drogon::Task<std::vector<dto::RegistrationInterestResponse>> GetAll() {
  auto interests{co_await repo::registration_interests::FindAll()};
  std::vector<dto::RegistrationInterestResponse> responses;
  responses.reserve(interests.size());
  for (auto&& ri : interests) {
    responses.push_back(MapToResponse(ri));
  }
  co_return responses;
}

drogon::Task<void> Delete(std::string_view id) {
  co_await repo::registration_interests::DeleteById(id);
}

}  // namespace service::registration_interests
