#pragma once
#include <vector>
#include <optional>
#include <string_view>
#include <string>

#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>

#include <dto/dto.h>

namespace service::product_images {

/**
 * @brief Create product image.
 */
[[nodiscard]] drogon::Task<dto::ProductImageResponse> Create(
    dto::CreateProductImageRequest request);

/**
 * @brief Update product image.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string_view image_id, dto::UpdateProductImageRequest request);

/**
 * @brief Delete product image.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string_view image_id);

/**
 * @brief Get product image by ID.
 */
[[nodiscard]] drogon::Task<std::optional<dto::ProductImageResponse>> GetById(
    std::string_view image_id);

/**
 * @brief Get all product images for a product.
 */
[[nodiscard]] drogon::Task<std::vector<dto::ProductImageResponse>> GetByProductId(
    std::string_view product_id);

/**
 * @brief Get active product images for a product.
 */
[[nodiscard]] drogon::Task<std::vector<dto::ProductImageResponse>> GetActiveByProductId(
    std::string_view product_id);

}  // namespace service::product_images
