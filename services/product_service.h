#pragma once
#include <drogon/drogon.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include "dto/dto.h"

namespace service::product {

drogon::Task<void> InitializeCache();

drogon::Task<void> RefreshCache(std::string product_id);

[[nodiscard]] drogon::Task<dto::ProductResponse> Create(
    dto::CreateProductRequest request);

[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateProductRequest request);

[[nodiscard]] drogon::Task<void> Delete(std::string id);

[[nodiscard]] std::optional<std::string> GetByIdJson(std::string_view id);

[[nodiscard]] std::string GetAllJson();

[[nodiscard]] std::vector<dto::ProductResponse> Search(
    dto::ProductSearchQuery query);

}  // namespace service::product
