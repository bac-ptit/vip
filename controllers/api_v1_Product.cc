#include "api_v1_Product.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> products::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProductRequest request) {
  try {
    auto response{co_await service::product::Create(std::move(request))};
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
    co_await service::product::Update(std::move(id), std::move(request));
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
