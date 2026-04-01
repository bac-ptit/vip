#pragma once
//
// Created by bac on 3/27/26.
//


#include <models/RegistrationInterests.h>





namespace domain {

class RegistrationInterests : public drogon_model::qlattt::RegistrationInterests {
 public:
  using drogon_model::qlattt::RegistrationInterests::RegistrationInterests;

  RegistrationInterests(drogon_model::qlattt::RegistrationInterests&& base) noexcept
      : drogon_model::qlattt::RegistrationInterests(std::move(base)) {}
};

}  // namespace domain
