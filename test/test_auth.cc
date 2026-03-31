#define DROGON_TEST_MAIN
#include <drogon/drogon_test.h>
#include <drogon/drogon.h>
#include <glaze/glaze.hpp>
import std;

using namespace drogon;

static std::string g_password = "Pass123!"; 

// Helper wrapper for POST requests in tests
void testPost(const HttpClientPtr& client, const std::string& path,
              std::map<std::string, std::string>&& body,   
              std::shared_ptr<drogon::test::Case> drogon_test_ctx_,  
              std::function<void(const HttpResponsePtr&)> checkFunc) {  
    auto req = HttpRequest::newHttpRequest();  
    req->setMethod(drogon::Post);  
    req->setPath(path);  
    req->setBody(glz::write_json(body).value_or("{}"));  
    req->setContentTypeCode(CT_APPLICATION_JSON);  
      
    client->sendRequest(req, [drogon_test_ctx_, checkFunc](ReqResult result, const HttpResponsePtr &resp) {  
        REQUIRE(result == ReqResult::Ok);  
        checkFunc(resp);  
    });  
}  

DROGON_TEST(AuthTest)
{
    auto client = HttpClient::newHttpClient("http://127.0.0.1:5555");
    
    // Define username and password at the start of test case
    std::string username = "bacdeptrai";
    std::string password = g_password;

    // 1. Register (Passes if created OR already exists)
    testPost(client, "/api/v1/auth/register", 
        {{"username", username}, {"password", password}, {"full_name", "Test"}}, 
        TEST_CTX,
        [TEST_CTX](const HttpResponsePtr& resp) {
            auto status = resp->getStatusCode();
            bool is_ok = (status == k200OK) || 
                         (status == k400BadRequest && resp->body() == "Username already exists");
            CHECK(is_ok);
        }
    );

    // 2. Login Success
    testPost(client, "/api/v1/auth/login", 
        {{"username", username}, {"password", password}}, 
        TEST_CTX,
        [TEST_CTX](const HttpResponsePtr& resp) {
            CHECK(resp->getStatusCode() == k200OK);
            CHECK(!resp->getHeader("X-Admin-Id").empty());
        }
    );
}

int main(int argc, char** argv) 
{
    if (argc > 1) {
        g_password = argv[1];
    }

    using namespace drogon;
    std::promise<void> p1;
    std::future<void> f1 = p1.get_future();

    std::thread thr([&]() {
        app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
        app().run();
    });

    f1.get();
    int status = test::run(argc, argv);

    app().getLoop()->queueInLoop([]() { app().quit(); });
    thr.join();
    return status;
}
