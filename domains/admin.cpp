//
// Created by bac on 3/27/26.
//
import std;
module domain;


import utils;
import custom_exception;

void domain::Admin::SetPassword(std::string_view password) {
  auto verified_new_password{ValidateNewPassword(password)};
  if (!verified_new_password) {
    throw std::runtime_error(verified_new_password.error().data());
  }
  setPasswordHash(std::move(verified_new_password.value()));
}

void domain::Admin::SetUsername(std::string_view username) {
  auto validated{IsValidUsername(username)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  setUsername(std::string(username));
}

void domain::Admin::SetUsername(std::string&& username) {
  auto validated{IsValidUsername(username)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  setUsername(std::move(username));
}

void domain::Admin::SetFullName(std::string_view full_name) {
  auto validated{IsValidFullName(full_name)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  setFullName(std::string(full_name));
}

void domain::Admin::SetFullName(std::string&& full_name) {
  auto validated{IsValidFullName(full_name)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  setFullName(std::move(full_name));
}

void domain::Admin::SetEmail(std::string_view email) {
  auto validated{IsValidEmail(email)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  // Assuming email is stored in a field - adjust based on your schema
  // If there's no email field in the model, you may need to add it
  // For now, this is a placeholder implementation
}

void domain::Admin::SetEmail(std::string&& email) {
  auto validated{IsValidEmail(email)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  // Placeholder for move version
}

void domain::Admin::SetPhoneNumber(std::string_view phone_number) {
  auto validated{IsValidPhoneNumber(phone_number)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  // Assuming phone is stored in a field - adjust based on your schema
}

void domain::Admin::SetPhoneNumber(std::string&& phone_number) {
  auto validated{IsValidPhoneNumber(phone_number)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  // Placeholder for move version
}

void domain::Admin::SetAddress(std::string_view address) {
  auto validated{IsValidAddress(address)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  // Assuming address is stored in a field - adjust based on your schema
}

void domain::Admin::SetAddress(std::string&& address) {
  auto validated{IsValidAddress(address)};
  if (!validated) {
    throw std::runtime_error(validated.error().data());
  }
  // Placeholder for move version
}

std::expected<std::string, std::string_view> domain::Admin::ValidateNewPassword(
    std::string_view new_password) const {
  if (auto validated{IsValidPassword(new_password)}; !validated) {
    return std::unexpected(validated.error());
  }
  auto hash{utils::password_hasher::HashPassword(new_password)};
  if (hash == getValueOfPasswordHash()) {
    return std::unexpected("New password is the same as old password");
  }
  return hash;
}

std::expected<bool, std::string_view> domain::Admin::IsValidPassword(
    std::string_view password) {
  static const std::regex password_pattern(
      "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d).{8,128}$");

  if (password.empty()) {
    return std::unexpected("Password cannot be empty");
  }
  if (!std::regex_match(std::string(password), password_pattern)) {
    return std::unexpected(
        "Password must be 8-128 characters and contain at least one uppercase "
        "letter, one lowercase letter, and one digit");
  }
  return true;
}

std::expected<bool, std::string_view> domain::Admin::IsValidUsername(
    std::string_view username) {
  static const std::regex username_pattern("^[a-zA-Z0-9_-]{3,50}$");

  if (username.empty()) {
    return std::unexpected("Username cannot be empty");
  }
  if (!std::regex_match(std::string(username), username_pattern)) {
    return std::unexpected(
        "Username must be 3-50 characters and contain only letters, numbers, "
        "underscores, and hyphens");
  }
  return true;
}

std::expected<bool, std::string_view> domain::Admin::IsValidFullName(
    std::string_view full_name) {
  static const std::regex name_pattern("^[a-zA-Z\\s'-]{2,100}$");

  if (full_name.empty()) {
    return std::unexpected("Full name cannot be empty");
  }
  if (!std::regex_match(std::string(full_name), name_pattern)) {
    return std::unexpected(
        "Full name must be 2-100 characters and contain only letters, spaces, "
        "hyphens, and apostrophes");
  }
  return true;
}

std::expected<bool, std::string_view> domain::Admin::IsValidEmail(
    std::string_view email) {
  static const std::regex email_pattern(
      "^[a-zA-Z0-9._%+-]{1,64}@[a-zA-Z0-9.-]{1,189}\\.[a-zA-Z]{2,}$");

  if (email.empty()) {
    return std::unexpected("Email cannot be empty");
  }
  if (!std::regex_match(std::string(email), email_pattern)) {
    return std::unexpected("Invalid email format (max 254 characters)");
  }
  return true;
}

std::expected<bool, std::string_view> domain::Admin::IsValidPhoneNumber(
    std::string_view phone_number) {
  static const std::regex phone_pattern(
      "^[+]?[(]?[0-9]{1,4}[)]?[-\\s.]?[(]?[0-9]{1,4}[)]?[-\\s.]?[0-9]{1,4}[-"
      "\\s.]?[0-9]{1,9}$");
  static const std::regex digits_only_pattern("[^0-9]");

  if (phone_number.empty()) {
    return std::unexpected("Phone number cannot be empty");
  }
  if (!std::regex_match(std::string(phone_number), phone_pattern)) {
    return std::unexpected("Invalid phone number format");
  }
  std::string digits_only =
      std::regex_replace(std::string(phone_number), digits_only_pattern, "");
  if (digits_only.length() < 10 || digits_only.length() > 15) {
    return std::unexpected("Phone number must contain 10-15 digits");
  }
  return true;
}

std::expected<bool, std::string_view> domain::Admin::IsValidAddress(
    std::string_view address) {
  static const std::regex address_pattern("^[a-zA-Z0-9\\s,.'-]{5,500}$");

  if (address.empty()) {
    return std::unexpected("Address cannot be empty");
  }
  if (!std::regex_match(std::string(address), address_pattern)) {
    return std::unexpected(
        "Address must be 5-500 characters and contain only alphanumeric "
        "characters, spaces, and punctuation (,.'-)");
  }
  return true;
}

bool domain::Admin::VerifyPassword(std::string_view password) const {
  return utils::password_hasher::IsValidPassword(
      std::string(password), getValueOfPasswordHash());
}
