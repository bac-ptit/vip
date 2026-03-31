#include "api_v1_ProductImages.h"
#include "plugins/ImgBBPlugin.h"
#include <glaze/glaze.hpp>
#include <trantor/utils/Logger.h>

using namespace api::v1;

Task<> productimages::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProductImageRequest request) {
  try {
    MultiPartParser fileUpload;
    if (fileUpload.parse(req) == 0 && !fileUpload.getFiles().empty()) {
      auto& file = fileUpload.getFiles()[0];
      auto* imgbb = drogon::app().getPlugin<custom_plugin::ImgBBPlugin>();
      if (imgbb) {
        LOG_INFO << "Uploading image to ImgBB...";
        auto content = file.fileContent();
        auto url = co_await imgbb->UploadImage(utils::base64Encode((const unsigned char*)content.data(), content.size()));
        request.url = std::move(url);
      }
    }

    auto response{co_await service::product_images::Create(std::move(request))};
    
    // Refresh Product Cache to include new image
    if (response.product_id) {
      co_await service::product::RefreshCache(*response.product_id);
    }

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

Task<> productimages::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProductImageRequest request) {
  try {
    auto existing = co_await service::product_images::GetById(id);
    co_await service::product_images::Update(id, std::move(request));
    
    // Refresh Product Cache
    if (existing && existing->product_id) {
      co_await service::product::RefreshCache(*existing->product_id);
    }

    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> productimages::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  try {
    auto existing = co_await service::product_images::GetById(id);
    co_await service::product_images::Delete(id);
    
    // Refresh Product Cache
    if (existing && existing->product_id) {
      co_await service::product::RefreshCache(*existing->product_id);
    }

    callback(HttpResponse::newHttpResponse());
  } catch (const std::exception& e) {
    auto resp{HttpResponse::newHttpResponse()};
    resp->setStatusCode(k400BadRequest);
    resp->setBody(e.what());
    callback(resp);
  }
  co_return;
}

Task<> productimages::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::product_images::GetById(id)};
  if (response) {
    auto json{glz::write_json(*response)};
    auto resp{HttpResponse::newHttpResponse()};
    resp->setBody(std::move(json).value_or("{}"));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    callback(resp);
    co_return;
  }
  callback(HttpResponse::newNotFoundResponse());
  co_return;
}

Task<> productimages::GetByProductId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string product_id) {
  auto response{co_await service::product_images::GetByProductId(product_id)};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> productimages::GetActiveByProductId(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string product_id) {
  auto response{co_await service::product_images::GetActiveByProductId(product_id)};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
