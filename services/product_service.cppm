//
// Created by bac on 3/27/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
export module service:product;
import std;


import dto;

export namespace service::product {

/**
 * @brief Initialize the product cache from the database.
 */
drogon::Task<void> InitializeCache();

/**
 * @brief Refresh cache for a single product.
 */
drogon::Task<void> RefreshCache(std::string product_id);

/**
 * @brief Create a new product with associated images and update the cache.
 */
[[nodiscard]] drogon::Task<dto::ProductResponse> Create(
    dto::CreateProductRequest request, std::vector<std::string> new_image_urls);

/**
 * @brief Update an existing product, add/delete images and refresh cache.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateProductRequest request, std::vector<std::string> new_image_urls);

/**
 * @brief Delete a product and its associated images, then update the cache.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string id);

/**
 * @brief Get product by ID (from cache).
 */
[[nodiscard]] std::optional<std::string> GetByIdJson(
    std::string_view id);

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
