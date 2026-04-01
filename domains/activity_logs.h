#pragma once
//
// Created by bac on 3/29/26.
//


#include <model.h>





namespace domain {

class ActivityLogs : public drogon_model::qlattt::ActivityLogs {
 public:
  using drogon_model::qlattt::ActivityLogs::ActivityLogs;

  ActivityLogs(drogon_model::qlattt::ActivityLogs&& base) noexcept
      : drogon_model::qlattt::ActivityLogs(std::move(base)) {}
};

}  // namespace domain
