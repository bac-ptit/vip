module;
#include <sodium.h>
import std;

module utils;

import :password_hasher;

namespace utils::password_hasher {

std::string HashPassword(std::string_view password) {
    if (sodium_init() < 0) {
        throw std::runtime_error("libsodium initialization failed");
    }

    char hashed_password[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(
            hashed_password,
            password.data(),
            password.length(),
            crypto_pwhash_OPSLIMIT_INTERACTIVE,
            crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        throw std::runtime_error("Password hashing failed");
    }

    return std::string(hashed_password);
}

bool IsValidPassword(const std::string& password, const std::string& hash) {
    if (sodium_init() < 0) {
        throw std::runtime_error("libsodium initialization failed");
    }

    return crypto_pwhash_str_verify(
               hash.c_str(),
               password.c_str(),
               password.length()) == 0;
}

} // namespace utils::password_hasher
