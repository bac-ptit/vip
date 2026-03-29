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

export module dto:activity_logs;

export namespace dto {

struct ActivityLogResponse {
  std::shared_ptr<std::string> id;
  std::shared_ptr<std::string> admin_id;
  std::shared_ptr<std::string> action;
  std::shared_ptr<std::string> entity;
  std::shared_ptr<std::string> entity_id;
  std::shared_ptr<std::string> detail;
  std::shared_ptr<std::string> ip_address;
  std::shared_ptr<trantor::Date> created_at;
};

}  // namespace dto
