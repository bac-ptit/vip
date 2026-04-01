#include <stdexcept>
#include <string>
#include "ImgBBPlugin.h"
#include <drogon/drogon.h>
#include <glaze/glaze.hpp>


using namespace custom_plugin;
using namespace drogon;

namespace {
struct ImgBBData {
  std::string url;
};
struct ImgBBResponse {
  ImgBBData data;
  bool success{false};
};
}

void ImgBBPlugin::initAndStart(const Json::Value& config) {
  // Lấy API Key và URL từ config plugin hoặc custom config
  api_key_ = config.get("api_key", "").asString();
  api_url_ = config.get("api_url", "https://api.imgbb.com").asString();
  
  if (api_key_.empty()) {
    auto custom_config{app().getCustomConfig()};
    api_key_ = custom_config.get("imgbb_api_key", "").asString();
    if (api_url_ == "https://api.imgbb.com") {
        api_url_ = custom_config.get("imgbb_api_url", "https://api.imgbb.com").asString();
    }
  }
  
  if (api_key_.empty()) {
    LOG_ERROR << "ImgBB API Key is missing!";
  } else {
    // Khởi tạo client một lần duy nhất để tái sử dụng
    client_ = HttpClient::newHttpClient(api_url_);
    LOG_INFO << "ImgBBPlugin initialized with host: " << api_url_;
  }
}

void ImgBBPlugin::shutdown() {
  LOG_INFO << "ImgBBPlugin shutting down.";
}

Task<std::string> ImgBBPlugin::UploadImage(std::string base64_data) {
  if (api_key_.empty() || !client_) {
    throw std::runtime_error("ImgBBPlugin is not properly configured.");
  }

  auto req{HttpRequest::newHttpRequest()};
  req->setMethod(drogon::Post);
  req->setPath("/1/upload");
  req->setContentTypeCode(CT_APPLICATION_X_FORM);
  
  req->setParameter("key", api_key_);
  req->setParameter("image", base64_data);

  try {
    auto resp{co_await client_->sendRequestCoro(req)};
    
    if (resp->getStatusCode() != k200OK) {
      LOG_ERROR << "ImgBB API Error: " << resp->body();
      throw std::runtime_error("ImgBB API returned error: " + std::to_string(resp->getStatusCode()));
    }

    ImgBBResponse res;
    auto error_code{glz::read_json(res, resp->body())};
    if (error_code) {
      throw std::runtime_error("Failed to parse ImgBB response.");
    }

    co_return res.data.url;
  } catch (const std::exception& error) {
    LOG_ERROR << "Exception during ImgBB upload: " << error.what();
    throw;
  }
}
