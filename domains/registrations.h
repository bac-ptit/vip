#pragma once
//
// Created by bac on 3/27/26.
//

#include <models/Registrations.h>

#include <expected>

namespace domain {

class Registrations : public drogon_model::qlattt::Registrations {
 public:
  using drogon_model::qlattt::Registrations::Registrations;

  explicit Registrations(drogon_model::qlattt::Registrations&& base) noexcept
      : drogon_model::qlattt::Registrations(std::move(base)) {}

  void setPhone(const std::string& phone);
  void setPhone(std::string&& phone);

private:
  static void ValidatePhoneNumber(std::string_view phone_number);
};


}  // namespace domain
