//
// Created by bac on 3/29/26.
//

module;
#include <model.h>

export module domain:admin_sessions;

export namespace domain {

class AdminSessions : public drogon_model::qlattt::AdminSessions {
 public:
  using drogon_model::qlattt::AdminSessions::AdminSessions;

  AdminSessions(drogon_model::qlattt::AdminSessions&& base) noexcept
      : drogon_model::qlattt::AdminSessions(std::move(base)) {}
};

}  // namespace domain
