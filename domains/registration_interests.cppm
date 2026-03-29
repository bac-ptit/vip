//
// Created by bac on 3/27/26.
//

module;
#include <models/RegistrationInterests.h>

export module domain:registration_interests;

export namespace domain {

class RegistrationInterests : public drogon_model::qlattt::RegistrationInterests {
 public:
  using drogon_model::qlattt::RegistrationInterests::RegistrationInterests;

  RegistrationInterests(drogon_model::qlattt::RegistrationInterests&& base) noexcept
      : drogon_model::qlattt::RegistrationInterests(std::move(base)) {}
};

}  // namespace domain
