//
// Created by bac on 3/27/26.
//

module;
#include <drogon/drogon.h>
#include <drogon/HttpTypes.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

export module service:product;

import dto;

export namespace service::product {

/**
 * @brief Initialize the product cache from the database.
 */
drogon::Task<void> InitializeCache();

/**
 * @brief Create a new product and update the cache.
 */
[[nodiscard]] drogon::Task<dto::ProductResponse> Create(
    dto::CreateProductRequest request);

/**
 * @brief Update an existing product and the cache.
 */
[[nodiscard]] drogon::Task<void> Update(
    std::string id, dto::UpdateProductRequest request);

/**
 * @brief Delete a product and update the cache.
 */
[[nodiscard]] drogon::Task<void> Delete(std::string id);

/**
 * @brief Get product by ID (from cache).
 */
[[nodiscard]] std::optional<dto::ProductResponse> GetById(
    std::string id);

/**
 * @brief Get all products (from cache).
 */
[[nodiscard]] std::vector<dto::ProductResponse> GetAll();

/**
 * @brief Search products in the cache.
 */
[[nodiscard]] std::vector<dto::ProductResponse> Search(
    dto::ProductSearchQuery query);

}  // namespace service::product
