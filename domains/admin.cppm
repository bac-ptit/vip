//
// Created by bac on 3/27/26.
//

module;
#include <model.h>
export module domain:admin;
import std;


export namespace domain {
/**
 * @brief Domain wrapper for administrator accounts.
 *
 * Provides validation helpers for admin fields and enforces
 * PascalCase setter naming (e.g. SetPassword).
 * Setter should use PascalCase. Because of name-hiring
 */
class Admin : public drogon_model::qlattt::Admins {
 public:
  using drogon_model::qlattt::Admins::Admins;

  Admin(drogon_model::qlattt::Admins&& base) noexcept
      : drogon_model::qlattt::Admins(std::move(base)) {}

  [[nodiscard]] static std::expected<bool, std::string_view> IsValidPassword(
      std::string_view password);

  [[nodiscard]] static std::expected<bool, std::string_view> IsValidUsername(
      std::string_view username);

  [[nodiscard]] static std::expected<bool, std::string_view> IsValidFullName(
      std::string_view full_name);

  [[nodiscard]] static std::expected<bool, std::string_view> IsValidEmail(
      std::string_view email);
  [[nodiscard]] static std::expected<bool, std::string_view> IsValidPhoneNumber(
      std::string_view phone_number);
  [[nodiscard]] static std::expected<bool, std::string_view> IsValidAddress(
      std::string_view address);

  void SetPassword(std::string_view password);
  void SetUsername(std::string_view username);
  void SetUsername(std::string&& username);
  void SetFullName(std::string_view full_name);
  void SetFullName(std::string&& full_name);
  void SetEmail(std::string_view email);
  void SetEmail(std::string&& email);
  void SetPhoneNumber(std::string_view phone_number);
  void SetPhoneNumber(std::string&& phone_number);
  void SetAddress(std::string_view address);
  void SetAddress(std::string&& address);

  [[nodiscard]] bool VerifyPassword(std::string_view password) const;

 private:

  [[nodiscard]] std::expected<std::string, std::string_view>
  ValidateNewPassword(std::string_view new_password) const;
};
}  // namespace domain
