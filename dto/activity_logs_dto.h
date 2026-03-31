#pragma once
#include <memory>
#include <string>
#include <trantor/utils/Date.h>

namespace dto {

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
