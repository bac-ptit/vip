//
// Created by bac on 3/28/26.
//

module;
#include <models/ProductImages.h>

export module domain:product_images;

export namespace domain {

class ProductImages : public drogon_model::qlattt::ProductImages {
 public:
  using drogon_model::qlattt::ProductImages::ProductImages;

  ProductImages(drogon_model::qlattt::ProductImages&& base) noexcept
      : drogon_model::qlattt::ProductImages(std::move(base)) {}
};

}  // namespace domain
