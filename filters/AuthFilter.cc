#include <drogon/HttpFilter.h>
#include <drogon/HttpTypes.h>
#include <drogon/Session.h>
#include "AuthFilter.h"

using namespace filter;
using namespace drogon;

AuthFilter::AuthFilter() = default;

void AuthFilter::doFilter(const HttpRequestPtr& req,
                         FilterCallback&& callback,
                         FilterChainCallback&& filter_chain_callback) {
  auto session{req->session()};
  if (session && (session->find("admin_id") || session->find("user_id"))) {
    std::move(filter_chain_callback)();
    return;
  }

  const auto& admin_id_header{req->getHeader("X-Admin-Id")};
  if (!admin_id_header.empty()) {
    std::move(filter_chain_callback)();
    return;
  }

  auto resp{HttpResponse::newHttpResponse()};
  resp->setStatusCode(k401Unauthorized);
  std::move(callback)(resp);
}
