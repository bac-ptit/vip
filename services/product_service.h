#pragma once
#include <vector>
#include <optional>
#include <string_view>
#include <string>

#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>

#include <dto/dto.h>

namespace service::product {

/**
 * @brief Initialize the product cache from the database.
 */
drogon::Task<void> InitializeCache();

/**
 * @brief Refresh cache for a single product.
 */
drogon::Task<void> RefreshCache(std::string product_id);

/**
 * @brief Create a new product and update the cache.
 */
[[nodiscard]] drogon::Task<dto::ProductResponse> Create(
    dto::CreateProductRequest request, std::vector<std::string> new_image_urls);

/**
 * @brief Update an existing product and refresh cache.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string product_id, dto::UpdateProductRequest request, std::vector<std::string> new_image_urls);

/**
 * @brief Delete a product and its associated images, then update the cache.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string product_id);

/**
 * @brief Get product by ID (from cache).
 */
[[nodiscard]] std::optional<std::string> GetByIdJson(
    std::string_view product_id);

/**
 * @brief Get all products (from cache as JSON array string).
 */
[[nodiscard]] std::string GetAllJson();

/**
 * @brief Search products in the cache.
 */
[[nodiscard]] std::vector<dto::ProductResponse> Search(
    dto::ProductSearchQuery query);

}  // namespace service::product
