#pragma once
#include <string>
//
// Created by bac on 3/27/26.
//


#include <trantor/utils/Date.h>
#include <glaze/glaze.hpp>
#include <string>




#include "admin_dto.h"
#include "product_dto.h"
#include "project_info_dto.h"
#include "media_dto.h"
#include "product_images_dto.h"
#include "floor_plans_dto.h"
#include "registrations_dto.h"
#include "registration_interests_dto.h"
#include "site_settings_dto.h"
#include "admin_sessions_dto.h"
#include "activity_logs_dto.h"


namespace dto {
struct ErrorResponse {
  std::string error;
};
}  // namespace dto


  namespace glz {
  // Chiều đọc: JSON -> trantor::Date
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
      static std::string fmt{"%Y-%m-%d %H:%M:%S"};
      auto s{value.toCustomFormattedStringLocal(fmt)};
      serialize<JSON>::op<Opts>(s, ctx, b, ix);
    }
  };
  }  // namespace glz
