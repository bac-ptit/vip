#include "AuthFilter.h"
#include <drogon/drogon.h>
#include <glaze/glaze.hpp>
#include <map>
#include <string>
#include <vector>

using namespace filter;

AuthFilter::AuthFilter() {}

void AuthFilter::doFilter(const HttpRequestPtr& req,
                          FilterCallback&& fcb,
                          FilterChainCallback&& fccb) {
  // 1. Kiểm tra trong Session (Drogon builtin)
  auto session{req->session()};
  if (session && (session->find("admin_id") || session->find("user_id"))) {
    fccb();
    return;
  }

  // 2. Kiểm tra trong Header (Custom X-Admin-Id)
  auto& admin_id_header{req->getHeader("X-Admin-Id")};
  if (!admin_id_header.empty()) {
    fccb();
    return;
  }

  // Unauthorized - Use Glaze for performance
  std::string json_str;
  (void)glz::write_json(std::map<std::string, std::string>{{"error", "Unauthorized: Please login"}}, json_str);
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json_str));
  resp->setStatusCode(k401Unauthorized);
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  fcb(resp);
}
