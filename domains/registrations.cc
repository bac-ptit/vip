#include "registrations.h"
#include <regex>

namespace domain {

void Registrations::MarkAsContacted(std::string admin_id) noexcept {
  setIsContacted(true);
  setContactedBy(std::move(admin_id));
  setContactedAt(trantor::Date::now());
}

void Registrations::setPhone(const std::string& phone) {
  ValidatePhoneNumber(phone);
  drogon_model::qlattt::Registrations::setPhone(phone);
}

void Registrations::setPhone(std::string&& phone) {
  ValidatePhoneNumber(phone);
  drogon_model::qlattt::Registrations::setPhone(std::move(phone));
}

void Registrations::ValidatePhoneNumber(std::string_view phone_number) {
  static const std::regex phone_pattern{R"(^((0\d{9})|(\+\d{11}))$)"};
  if (!std::regex_match(phone_number.begin(), phone_number.end(), phone_pattern)) {
    throw std::runtime_error("Invalid phone number format");
  }
}

}  // namespace domain
