#pragma once
#include <models/Media.h>

namespace domain {

class Media : public drogon_model::qlattt::Media {
 public:
  using drogon_model::qlattt::Media::Media;

  Media(drogon_model::qlattt::Media&& base) noexcept
      : drogon_model::qlattt::Media(std::move(base)) {}
};

}  // namespace domain
