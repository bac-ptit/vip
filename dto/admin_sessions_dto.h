#pragma once
#include <memory>
#include <string>
#include <trantor/utils/Date.h>

namespace dto {

struct AdminSessionResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> admin_id;
  std::shared_ptr<std::string> refresh_token;
  std::shared_ptr<std::string> ip_address;
  std::shared_ptr<std::string> user_agent;
  std::shared_ptr<trantor::Date> expires_at;
  std::shared_ptr<bool> revoked;
  std::shared_ptr<trantor::Date> created_at;
};

}  // namespace dto
