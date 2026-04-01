#pragma once
#include <vector>
#include <optional>
#include <string_view>
#include <string>
#include <drogon/HttpTypes.h>
#include <domains/domain.h>

namespace repo::product_images {

[[nodiscard]] drogon::Task<domain::ProductImages> Create(const domain::ProductImages& product_images);
[[nodiscard]] drogon::Task<std::optional<domain::ProductImages>> FindById(std::string_view image_id);
[[nodiscard]] drogon::Task<std::vector<domain::ProductImages>> FindByProductId(std::string_view product_id);
[[nodiscard]] drogon::Task<std::vector<domain::ProductImages>> FindAll();
[[nodiscard]] drogon::Task<size_t> Update(const domain::ProductImages& product_images);
[[nodiscard]] drogon::Task<bool> DeleteById(std::string_view image_id);

}  // namespace repo::product_images
