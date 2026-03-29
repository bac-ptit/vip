//
// Created by bac on 3/27/26.
//
module;
#include <string>

export module utils:password_hasher;


export namespace utils::password_hasher {

/**
 * Hashes a password using libsodium's Argon2id algorithm
 * @param password The plain text password to hash
 * @return The hashed password as a string, or empty string on failure
 */
std::string HashPassword(std::string_view password);

/**
 * Validates a password against a stored hash
 * @param password The plain text password to verify
 * @param hash The stored hash to verify against
 * @return true if the password matches the hash, false otherwise
 */
bool IsValidPassword(const std::string& password, const std::string& hash);

}
