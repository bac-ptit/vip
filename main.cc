#include <drogon/drogon.h>
import std;

import service;
import custom_exception;


int main() {
    drogon::app().loadConfigFile("../config.yaml");
    drogon::app().getLoop()->queueInLoop([]() {
        (void)service::product::InitializeCache();
    });

  const std::string frontend_url{drogon::app().getCustomConfig()["frontendUrl"].asString()};
  drogon::app().setExceptionHandler(exception::HandleException);

  drogon::app().registerPreRoutingAdvice(
      [&](const drogon::HttpRequestPtr& req,
                    drogon::AdviceCallback&& acb,
                    drogon::AdviceChainCallback&& accb) {
          if (req->method() == drogon::Options) {
              const auto resp{drogon::HttpResponse::newHttpResponse()};
              resp->addHeader("Access-Control-Allow-Methods",
                             "GET,POST,PUT,DELETE,OPTIONS,PATCH");
              resp->addHeader("Access-Control-Allow-Headers",
                             "Content-Type, Authorization, X-Requested-With");
              resp->addHeader("Access-Control-Allow-Credentials", "true");
              resp->addHeader("Access-Control-Max-Age", "86400"); // 24 hours
             resp->addHeader("Access-Control-Allow-Origin",
                          frontend_url);
              // Origin sẽ được thêm bởi PreSendingAdvice
              acb(resp);
              return;
          }
          accb();
      });

  // PreSendingAdvice cho tất cả responses (Bao gồm cả OPTIONS từ PreRoutingAdvice)
  drogon::app().registerPreSendingAdvice(
      [&](const drogon::HttpRequestPtr& req,
                    const drogon::HttpResponsePtr& resp) {
              resp->addHeader("Access-Control-Allow-Origin", frontend_url);
              resp->addHeader("Access-Control-Allow-Credentials", "true");
      });

    // Run HTTP framework
    drogon::app().run();
    return 0;
}
