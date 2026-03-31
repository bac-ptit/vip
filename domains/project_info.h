#pragma once
#include <models/ProjectInfo.h>

namespace domain {

class ProjectInfo : public drogon_model::qlattt::ProjectInfo {
 public:
  using drogon_model::qlattt::ProjectInfo::ProjectInfo;

  ProjectInfo(drogon_model::qlattt::ProjectInfo&& base) noexcept
      : drogon_model::qlattt::ProjectInfo(std::move(base)) {}
};

}  // namespace domain
