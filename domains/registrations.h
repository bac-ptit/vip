#pragma once
#include <models/Registrations.h>

namespace domain {

class Registrations : public drogon_model::qlattt::Registrations {
 public:
  using drogon_model::qlattt::Registrations::Registrations;

  Registrations(drogon_model::qlattt::Registrations&& base) noexcept
      : drogon_model::qlattt::Registrations(std::move(base)) {}
};

}  // namespace domain
