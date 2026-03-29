//
// Created by bac on 3/27/26.
//
module;
#include <sodium.h>
#include <string>

module utils;


namespace utils::password_hasher {

std::string HashPassword(std::string_view password) {
  if (password.empty()) {
    return "";
  }

  std::string hashed_password;
  hashed_password.resize(crypto_pwhash_STRBYTES);

  if (crypto_pwhash_str(hashed_password.data(), password.cbegin(),
                        password.length(), crypto_pwhash_OPSLIMIT_MODERATE,
                        crypto_pwhash_MEMLIMIT_MODERATE) != 0) {
    return {};
  }

  return hashed_password;
}

bool IsValidPassword(const std::string& password, const std::string& hash) {
  if (password.empty() || hash.empty()) {
    return false;
  }

  return crypto_pwhash_str_verify(hash.c_str(), password.c_str(),
                                  password.length()) == 0;
}
}  // namespace password_hasher
