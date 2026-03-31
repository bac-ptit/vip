
#include "api_v1_Product.h"
#include "plugins/ImgBBPlugin.h"
#include <glaze/glaze.hpp>
#include <trantor/utils/Logger.h>
import std;

using namespace api::v1;

Task<> products::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProductRequest request) {
  try {
    // 1. Upload multiple images via ImgBB
    std::vector<std::string> new_image_urls;
    MultiPartParser mp;
    if (mp.parse(req) == 0) {
        auto* imgbb = drogon::app().getPlugin<custom_plugin::ImgBBPlugin>();
        for (auto& file : mp.getFiles()) {
            if (imgbb) {
                auto content = file.fileContent();
                auto url = co_await imgbb->UploadImage(utils::base64Encode((const unsigned char*)content.data(), content.size()));
                new_image_urls.push_back(std::move(url));
            }
        }
    }

    // 2. Call service
    auto response{co_await service::product::Create(std::move(request), std::move(new_image_urls))};
    
    // 3. Response
    auto json{glz::write_json(response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> products::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProductRequest request) {
  try {
    // 1. Upload new images
    std::vector<std::string> new_image_urls;
    MultiPartParser mp;
    if (mp.parse(req) == 0) {
        auto* imgbb = drogon::app().getPlugin<custom_plugin::ImgBBPlugin>();
        for (auto& file : mp.getFiles()) {
            if (imgbb) {
                auto content = file.fileContent();
                auto url = co_await imgbb->UploadImage(utils::base64Encode((const unsigned char*)content.data(), content.size()));
                new_image_urls.push_back(std::move(url));
            }
        }
    }

    // 2. Call service
    co_await service::product::Update(std::move(id), std::move(request), std::move(new_image_urls));
    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> products::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::product::Delete(id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> products::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto json_opt{service::product::GetByIdJson(id)};
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
  query.name = req->getParameter("name");
  query.type = req->getParameter("type");
  query.min_price = req->getParameter("min_price");
  query.max_price = req->getParameter("max_price");

  auto response{service::product::Search(std::move(query))};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("[]"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
