#pragma once
#include <trantor/utils/Date.h>
#include <glaze/glaze.hpp>

#include "dto/admin_dto.h"
#include "dto/product_dto.h"
#include "dto/project_info_dto.h"
#include "dto/media_dto.h"
#include "dto/product_images_dto.h"
#include "dto/floor_plans_dto.h"
#include "dto/registrations_dto.h"
#include "dto/registration_interests_dto.h"
#include "dto/site_settings_dto.h"
#include "dto/admin_sessions_dto.h"
#include "dto/activity_logs_dto.h"

namespace dto {
struct ErrorResponse {
  std::string error;
};
}  // namespace dto

namespace glz {

template <>
struct from<JSON, trantor::Date> {
  template <auto Opts>
  static void op(trantor::Date& value, is_context auto&& ctx, auto&& it,
                 auto&& end) {
    std::string s;
    parse<JSON>::op<Opts>(s, ctx, it, end);
    value = trantor::Date::fromISOString(s);
  }
};

template <>
struct to<JSON, trantor::Date> {
  template <auto Opts>
  static void op(const trantor::Date& value, is_context auto&& ctx, auto&& b,
                 auto&& ix) noexcept {
    std::string s = value.toFormattedString(false);
    serialize<JSON>::op<Opts>(s, ctx, b, ix);
  }
};

}  // namespace glz
