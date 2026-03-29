//
// Created by bac on 3/27/26.
//

module;
#include <trantor/utils/Date.h>

#include <glaze/glaze.hpp>

export module dto;

export import :admin;
export import :product;
export import :project_info;
export import :media;
export import :product_images;
export import :floor_plans;
export import :registrations;
export import :registration_interests;
export import :site_settings;
export import :admin_sessions;
export import :activity_logs;
export namespace dto {
struct ErrorResponse {
  std::string error;
};
}  // namespace dto

export {
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
      std::string s = value.toFormattedString(false);
      serialize<JSON>::op<Opts>(s, ctx, b, ix);
    }
  };
  }  // namespace glz
}
