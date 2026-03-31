#pragma once
#include <drogon/drogon.h>
#include <glaze/glaze.hpp>
#include <stdexcept>
#include <string>
#include "dto/dto.h"

namespace exception {

class ProjectException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

inline void HandleException(
    const std::exception& e, const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
  const auto res{drogon::HttpResponse::newHttpResponse()};

  drogon::HttpStatusCode status{drogon::k500InternalServerError};

  if (dynamic_cast<const std::invalid_argument*>(&e)) [[unlikely]] {
    status = drogon::k400BadRequest;
  }

  res->setStatusCode(status);

  const dto::ErrorResponse error_dto{e.what()};
  res->setBody(glz::write_json(error_dto).value());
  res->setContentTypeCode(drogon::CT_APPLICATION_JSON);
  callback(res);
}

}  // namespace exception
