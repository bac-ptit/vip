#pragma once

#include <drogon/plugins/Plugin.h>
#include <drogon/drogon.h>
#include <string>
#include <memory>

namespace custom_plugin {

struct ImgBBData {
  std::string url;
};

struct ImgBBResponse {
  ImgBBData data;
  bool success;
};

class ImgBBPlugin : public drogon::Plugin<ImgBBPlugin> {
 public:
  ImgBBPlugin() = default;
  void initAndStart(const Json::Value& config) override;
  void shutdown() override;

  /**
   * @brief Upload an image to ImgBB.
   * @param base64_data Base64 encoded image data.
   * @return Task<std::string> The URL of the uploaded image.
   */
  [[nodiscard]] drogon::Task<std::string> UploadImage(std::string base64_data);

 private:
  std::string api_key_;
  std::string api_url_;
  drogon::HttpClientPtr client_;
};

}  // namespace custom_plugin
