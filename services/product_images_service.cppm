//
// Created by bac on 3/28/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
export module service:product_images;
import std;


import dto;

export namespace service::product_images {

/**
 * @brief Create product image.
 */
[[nodiscard]] drogon::Task<dto::ProductImageResponse> Create(
    dto::CreateProductImageRequest request);

/**
 * @brief Update product image.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string_view id, dto::UpdateProductImageRequest request);

/**
 * @brief Delete product image.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string_view id);

/**
 * @brief Get product image by ID.
 */
[[nodiscard]] drogon::Task<std::optional<dto::ProductImageResponse>> GetById(
    std::string_view id);

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
