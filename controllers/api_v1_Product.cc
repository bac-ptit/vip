#include "api_v1_Product.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> products::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProductRequest request) {
  auto response{co_await service::product::Create(std::move(request))};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> products::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProductRequest request) {
  co_await service::product::Update(std::move(id), std::move(request));
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> products::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::product::Delete(std::move(id));
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> products::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{service::product::GetById(std::move(id))};
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

Task<> products::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{service::product::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> products::Search(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  dto::ProductSearchQuery query;
  auto name{req->getParameter("name")};
  if (!name.empty()) query.name = name;
  auto type{req->getParameter("type")};
  if (!type.empty()) query.type = type;
  auto min_price{req->getParameter("min_price")};
  if (!min_price.empty()) query.min_price = min_price;
  auto max_price{req->getParameter("max_price")};
  if (!max_price.empty()) query.max_price = max_price;

  auto response{service::product::Search(query)};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
