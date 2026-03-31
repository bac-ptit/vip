//
// Created by bac on 3/27/26.
//

#include <drogon/HttpTypes.h>
#include <models/RegistrationInterests.h>
#include <optional>
#include <string>
#include <vector>
#include <ranges>
#include "services/registration_interests_service.h"
#include "dto/dto.h"
#include "repositories/repo.h"
#include "domains/domain.h"

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
  ri.setRegistrationId(std::move(request.registration_id));
  ri.setProductType(std::move(request.product_type));

  auto created_ri{co_await repo::registration_interests::Create(std::move(ri))};
  co_return MapToResponse(created_ri);
}

drogon::Task<std::vector<dto::RegistrationInterestResponse>>
GetByRegistrationId(std::string registration_id) {
  auto interests{
      co_await repo::registration_interests::FindByRegistrationId(std::move(registration_id))};
  std::vector<dto::RegistrationInterestResponse> responses;
  responses.reserve(interests.size());
  
  std::ranges::transform(interests, std::back_inserter(responses), MapToResponse);
  
  co_return responses;
}

drogon::Task<std::vector<dto::RegistrationInterestResponse>> GetAll() {
  auto interests{co_await repo::registration_interests::FindAll()};
  std::vector<dto::RegistrationInterestResponse> responses;
  responses.reserve(interests.size());
  
  std::ranges::transform(interests, std::back_inserter(responses), MapToResponse);
  
  co_return responses;
}

drogon::Task<void> Delete(std::string id) {
  co_await repo::registration_interests::DeleteById(std::move(id));
}

}  // namespace service::registration_interests
