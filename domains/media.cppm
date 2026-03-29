//
// Created by bac on 3/28/26.
//

module;
#include <models/Media.h>

export module domain:media;

export namespace domain {

class Media : public drogon_model::qlattt::Media {
 public:
  using drogon_model::qlattt::Media::Media;

  Media(drogon_model::qlattt::Media&& base) noexcept
      : drogon_model::qlattt::Media(std::move(base)) {}
};

}  // namespace domain
