#pragma once
#include <drogon/drogon.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include "dto/dto.h"

namespace service::product_images {

[[nodiscard]] drogon::Task<dto::ProductImageResponse> Create(
    dto::CreateProductImageRequest request);

[[nodiscard]] drogon::Task<void> Update(
    std::string_view id, dto::UpdateProductImageRequest request);

[[nodiscard]] drogon::Task<void> Delete(std::string_view id);

[[nodiscard]] drogon::Task<std::optional<dto::ProductImageResponse>> GetById(
    std::string_view id);

[[nodiscard]] drogon::Task<std::vector<dto::ProductImageResponse>> GetByProductId(
    std::string_view product_id);

[[nodiscard]] drogon::Task<std::vector<dto::ProductImageResponse>> GetActiveByProductId(
    std::string_view product_id);

}  // namespace service::product_images
