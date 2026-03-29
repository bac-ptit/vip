#include "api_v1_ProjectInfo.h"
#include <glaze/glaze.hpp>

using namespace api::v1;

Task<> projectinfo::Create(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, dto::CreateProjectInfoRequest&& request) {
  auto response{co_await service::project_info::Create(std::move(request))};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}

Task<> projectinfo::Update(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id, dto::UpdateProjectInfoRequest&& request) {
  co_await service::project_info::Update(id, std::move(request));
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> projectinfo::Delete(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  co_await service::project_info::Delete(id);
  callback(HttpResponse::newHttpResponse());
  co_return;
}

Task<> projectinfo::GetById(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback, std::string id) {
  auto response{co_await service::project_info::GetById(id)};
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

Task<> projectinfo::GetAll(HttpRequestPtr req, std::function<void(const HttpResponsePtr&)> callback) {
  auto response{co_await service::project_info::GetAll()};
  auto json{glz::write_json(response)};
  auto resp{HttpResponse::newHttpResponse()};
  resp->setBody(std::move(json).value_or("{}"));
  resp->setContentTypeCode(CT_APPLICATION_JSON);
  callback(resp);
  co_return;
}
