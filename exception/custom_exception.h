#pragma once
//
// Created by bac on 3/27/26.
//


#include <drogon/drogon.h>
#include <glaze/glaze.hpp>



#include <dto/dto.h>

namespace exception {

class ProjectException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

inline void HandleException(
    const std::exception& e, const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
  const auto res{drogon::HttpResponse::newHttpResponse()};

  // Default to 500
  drogon::HttpStatusCode status{drogon::k500InternalServerError};

  // Perform single dynamic_cast to AppException for project-specific errors
 if (dynamic_cast<const std::invalid_argument*>(&e)) [[unlikely]] {
    // Map standard invalid_argument (from validation logic) to 400
    status = drogon::k400BadRequest;
  }

  res->setStatusCode(status);

  const dto::ErrorResponse error_dto{e.what()};
  res->setBody(glz::write_json(error_dto).value());
  res->setContentTypeCode(drogon::CT_APPLICATION_JSON);
  callback(res);
}

}  // namespace exception
