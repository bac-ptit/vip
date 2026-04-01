#pragma once
#include <drogon/HttpFilter.h>
#include <drogon/HttpTypes.h>

using namespace drogon;

namespace filter {

class AuthFilter : public drogon::HttpFilter<AuthFilter> {
 public:
  AuthFilter();
  void doFilter(const HttpRequestPtr& req,
                FilterCallback&& callback,
                FilterChainCallback&& filter_chain_callback) override;
};

}  // namespace filter
