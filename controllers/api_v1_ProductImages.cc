#include "api_v1_ProductImages.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> productimages::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProductImageRequest&& request) {
  auto response{co_await service::product_images::Create(std::move(request))};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> productimages::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProductImageRequest&& request) {
  co_await service::product_images::Update(id, std::move(request));
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> productimages::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::product_images::Delete(id);
  callback(HttpResponse::newHttpResponse());
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
