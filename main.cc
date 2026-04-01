#include <drogon/drogon.h>
#include <services/service.h>
#include <exception/custom_exception.h>
#include <string>

using namespace drogon;

int main() {
    drogon::app().loadConfigFile("../config.yaml");
    drogon::app().getLoop()->queueInLoop([]() {
        (void)service::product::InitializeCache();
    });

  const std::string frontend_url{drogon::app().getCustomConfig()["frontendUrl"].asString()};
  drogon::app().setExceptionHandler(exception::HandleException);

  drogon::app().registerPreRoutingAdvice(
      [frontend_url](const HttpRequestPtr& req,
                    const AdviceCallback& acb,
                    const AdviceChainCallback& accb) {
          if (req->method() == Options) {
              const auto resp{HttpResponse::newHttpResponse()};
              resp->addHeader("Access-Control-Allow-Origin", frontend_url);
              resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, DELETE");
              resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Admin-Id");
              resp->addHeader("Access-Control-Allow-Credentials", "true");
              acb(resp);
              return;
          }
          accb();
      });

  drogon::app().registerPostHandlingAdvice(
      [frontend_url](const HttpRequestPtr& req,
                    const HttpResponsePtr& resp) {
              (void)req;
              resp->addHeader("Access-Control-Allow-Origin", frontend_url);
              resp->addHeader("Access-Control-Allow-Credentials", "true");
      });

    // Run HTTP framework
    drogon::app().run();
    return 0;
}
