//
// Created by bac on 3/28/26.
//

module;
#include <models/ProjectInfo.h>

export module domain:project_info;

export namespace domain {

class ProjectInfo : public drogon_model::qlattt::ProjectInfo {
 public:
  using drogon_model::qlattt::ProjectInfo::ProjectInfo;

  ProjectInfo(drogon_model::qlattt::ProjectInfo&& base) noexcept
      : drogon_model::qlattt::ProjectInfo(std::move(base)) {}
};

}  // namespace domain
