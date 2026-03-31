#pragma once
#include <models/ProductImages.h>

namespace domain {

class ProductImages : public drogon_model::qlattt::ProductImages {
 public:
  using drogon_model::qlattt::ProductImages::ProductImages;

  ProductImages(drogon_model::qlattt::ProductImages&& base) noexcept
      : drogon_model::qlattt::ProductImages(std::move(base)) {}
};

}  // namespace domain
