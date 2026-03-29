//
// Created by bac on 3/27/26.
//

module;
#include <models/Registrations.h>

export module domain:registrations;

export namespace domain {

class Registrations : public drogon_model::qlattt::Registrations {
 public:
  using drogon_model::qlattt::Registrations::Registrations;

  Registrations(drogon_model::qlattt::Registrations&& base) noexcept
      : drogon_model::qlattt::Registrations(std::move(base)) {}
};

}  // namespace domain
