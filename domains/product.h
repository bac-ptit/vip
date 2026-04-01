#pragma once
//
// Created by bac on 3/27/26.
//


#include <models/Products.h>





namespace domain {

class Product : public drogon_model::qlattt::Products {
 public:
  using drogon_model::qlattt::Products::Products;

  Product(drogon_model::qlattt::Products&& base) noexcept
      : drogon_model::qlattt::Products(std::move(base)) {}
  
  // Custom business logic can be added here
};

}  // namespace domain
