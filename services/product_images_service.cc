#include "product_images_service.h"
#include <repositories/product_images_repository.h>
#include <domains/product_images.h>
#include <algorithm>
#include <ranges>

namespace service::product_images {

static dto::ProductImageResponse ToProductImageResponse(const domain::ProductImages& image) {
  return {
      .id = image.getId(),
      .product_id = image.getProductId(),
      .url = image.getUrl(),
      .is_cover = image.getIsCover(),
      .sort_order = image.getSortOrder(),
      .uploaded_by = image.getUploadedBy(),
      .created_at = image.getCreatedAt()};
}

drogon::Task<dto::ProductImageResponse> Create(
    dto::CreateProductImageRequest request) {
  domain::ProductImages image;
  image.setProductId(std::move(request.product_id));
  image.setUrl(std::move(request.url));
  image.setIsCover(request.is_cover);
  image.setSortOrder(request.sort_order);

  auto created{co_await repo::product_images::Create(std::move(image))};
  co_return ToProductImageResponse(created);
}

drogon::Task<void> Update(
    std::string_view image_id, dto::UpdateProductImageRequest request) {
  auto existing_opt{co_await repo::product_images::FindById(image_id)};
  if (!existing_opt) {
    throw std::runtime_error{"Product image not found"};
  }

  auto existing{std::move(*existing_opt)};

  if (request.product_id) existing.setProductId(std::move(*request.product_id));
  if (request.url) existing.setUrl(std::move(*request.url));
  if (request.is_cover) existing.setIsCover(*request.is_cover);
  if (request.sort_order) existing.setSortOrder(*request.sort_order);

  co_await repo::product_images::Update(existing);
}

drogon::Task<void> Delete(std::string_view image_id) {
  co_await repo::product_images::DeleteById(image_id);
}

drogon::Task<std::optional<dto::ProductImageResponse>> GetById(
    std::string_view image_id) {
  auto image_opt{co_await repo::product_images::FindById(image_id)};
  if (image_opt) {
    co_return ToProductImageResponse(std::move(*image_opt));
  }
  co_return std::nullopt;
}

drogon::Task<std::vector<dto::ProductImageResponse>> GetByProductId(
    std::string_view product_id) {
  auto images_raw{co_await repo::product_images::FindByProductId(product_id)};
  std::vector<dto::ProductImageResponse> result;
  result.reserve(images_raw.size());
  
  std::ranges::transform(images_raw, std::back_inserter(result), ToProductImageResponse);
  
  co_return result;
}

drogon::Task<std::vector<dto::ProductImageResponse>> GetActiveByProductId(
    std::string_view product_id) {
  // Current model doesn't have is_active for ProductImages, returning all for now
  co_return co_await GetByProductId(product_id);
}

}  // namespace service::product_images
