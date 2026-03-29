#pragma once

#include <drogon/HttpFilter.h>

using namespace drogon;

namespace filter {

/**
 * @brief Authentication filter for VIP backend.
 * Checks session and validates against exempted paths in config.yaml.
 */
class AuthFilter : public HttpFilter<AuthFilter> {
 public:
  AuthFilter();
  void doFilter(const HttpRequestPtr& req,
                FilterCallback&& fcb,
                FilterChainCallback&& fccb) override;
};

}  // namespace filter
