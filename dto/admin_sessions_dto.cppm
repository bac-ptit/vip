//
// Created by bac on 3/29/26.
//

module;
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <glaze/glaze.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <trantor/utils/Date.h>
#include <vector>

export module dto:admin_sessions;

export namespace dto {

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
