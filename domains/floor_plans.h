#pragma once
//
// Created by bac on 3/28/26.
//


#include <models/FloorPlans.h>





namespace domain {

class FloorPlans : public drogon_model::qlattt::FloorPlans {
 public:
  using drogon_model::qlattt::FloorPlans::FloorPlans;

  FloorPlans(drogon_model::qlattt::FloorPlans&& base) noexcept
      : drogon_model::qlattt::FloorPlans(std::move(base)) {}
};

}  // namespace domain
