#include "api_v1_Product.h"
#include <glaze/glaze.hpp>
#include <plugins/ImgBBPlugin.h>
#include <drogon/MultiPart.h>
#include <drogon/utils/Utilities.h>

using namespace api::v1;

Task<> products::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProductRequest request) {
    auto* imgbb{drogon::app().getPlugin<custom_plugin::ImgBBPlugin>()};
    std::vector<std::string> image_urls;

    MultiPartParser parser;
    if (parser.parse(req) == 0) {
        const auto& files = parser.getFiles();
        for (const auto& file : files) {
            if (imgbb != nullptr) {
                auto content{file.fileContent()};
                auto url{co_await imgbb->UploadImage(utils::base64Encode(reinterpret_cast<const unsigned char*>(content.data()), content.size()))}; // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
                image_urls.push_back(std::move(url));
            }
        }
    }

    auto response_dto{co_await service::product::Create(std::move(request), std::move(image_urls))};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k201Created);
    resp->setBody(glz::write_json(response_dto).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
}

Task<> products::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string product_id, dto::UpdateProductRequest request) {
    auto* imgbb{drogon::app().getPlugin<custom_plugin::ImgBBPlugin>()};
    std::vector<std::string> image_urls;

    MultiPartParser parser;
    if (parser.parse(req) == 0) {
        const auto& files = parser.getFiles();
        for (const auto& file : files) {
            if (imgbb != nullptr) {
                auto content{file.fileContent()};
                auto url{co_await imgbb->UploadImage(utils::base64Encode(reinterpret_cast<const unsigned char*>(content.data()), content.size()))}; // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
                image_urls.push_back(std::move(url));
            }
        }
    }

    co_await service::product::Update(std::move(product_id), std::move(request), std::move(image_urls));
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k204NoContent);
    callback(resp);
}

Task<> products::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string product_id) {
  co_await service::product::Delete(std::move(product_id));
  
  auto resp{HttpResponse::newHttpResponse()};
  resp->setStatusCode(k204NoContent);
  callback(resp);
}

Task<> products::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string product_id) {
  auto json_opt{service::product::GetByIdJson(product_id)};
  if (json_opt) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(*json_opt));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } else {
    callback(HttpResponse::newNotFoundResponse());
  }
  co_return;
}

Task<> products::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
    auto json{service::product::GetAllJson()};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
    co_return;
}

Task<> products::Search(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
    dto::ProductSearchQuery query;
    query.name = req->getOptionalParameter<std::string>("name");
    query.type = req->getOptionalParameter<std::string>("type");
    query.min_price = req->getOptionalParameter<std::string>("min_price");
    query.max_price = req->getOptionalParameter<std::string>("max_price");

    auto results{service::product::Search(std::move(query))};
    
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(glz::write_json(results).value_or("[]"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
    co_return;
}
