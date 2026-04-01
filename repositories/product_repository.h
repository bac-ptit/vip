#pragma once
#include <vector>
#include <optional>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::product {

[[nodiscard]] drogon::Task<domain::Product> Create(const domain::Product& p);
[[nodiscard]] drogon::Task<std::optional<domain::Product>> FindById(std::string productId);
[[nodiscard]] drogon::Task<std::vector<domain::Product>> FindAll();
[[nodiscard]] drogon::Task<std::vector<domain::Product>> FindActive();
[[nodiscard]] drogon::Task<size_t> Update(const domain::Product& p);
[[nodiscard]] drogon::Task<bool> DeleteById(std::string productId);

}  // namespace repo::product
