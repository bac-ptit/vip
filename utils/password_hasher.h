#pragma once
#include <string>

namespace utils::password_hasher {

std::string HashPassword(std::string_view password);

bool IsValidPassword(const std::string& password, const std::string& hash);

}  // namespace utils::password_hasher
