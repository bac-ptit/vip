#include <utility>
#include <vector>
#include <optional>
#include <string>
#include <ranges>
#include "admin_sessions_service.h"

#include <drogon/drogon.h>
#include <models/AdminSessions.h>

#include <domains/domain.h>
#include <repositories/repo.h>
#include <dto/dto.h>

using namespace drogon;

static dto::AdminSessionResponse ToAdminSessionResponse(const domain::AdminSessions& session) {
  dto::AdminSessionResponse response;
  response.id = session.getId();
  response.admin_id = session.getAdminId();
  response.refresh_token = session.getRefreshToken();
  response.ip_address = session.getIpAddress();
  response.user_agent = session.getUserAgent();
  response.expires_at = session.getExpiresAt();
  response.revoked = session.getRevoked();
  response.created_at = session.getCreatedAt();
  return response;
}

drogon::Task<std::optional<dto::AdminSessionResponse>> service::admin_sessions::GetById(std::string session_id) {
  auto session{co_await repo::admin_sessions::FindById(std::move(session_id))};
  if (!session) {
    co_return std::nullopt;
  }
  co_return ToAdminSessionResponse(*session);
}

drogon::Task<std::optional<dto::AdminSessionResponse>> service::admin_sessions::GetByRefreshToken(std::string token) {
  auto session{co_await repo::admin_sessions::FindByRefreshToken(std::move(token))};
  if (!session) {
    co_return std::nullopt;
  }
  co_return ToAdminSessionResponse(*session);
}

drogon::Task<std::vector<dto::AdminSessionResponse>> service::admin_sessions::GetByAdminId(std::string admin_id) {
  auto sessions{co_await repo::admin_sessions::FindByAdminId(std::move(admin_id))};
  std::vector<dto::AdminSessionResponse> responses;
  responses.reserve(sessions.size());
  
  std::ranges::transform(sessions, std::back_inserter(responses), ToAdminSessionResponse);
  
  co_return responses;
}

drogon::Task<void> service::admin_sessions::RevokeSession(std::string session_id) {
  auto session{co_await repo::admin_sessions::FindById(std::move(session_id))};
  if (session) {
    session->setRevoked(true);
    co_await repo::admin_sessions::Update(*session);
  }
}

drogon::Task<void> service::admin_sessions::RevokeAllAdminSessions(std::string admin_id) {
  auto sessions{co_await repo::admin_sessions::FindByAdminId(std::move(admin_id))};
  for (auto& s : sessions) {
    if (!s.getValueOfRevoked()) {
      s.setRevoked(true);
      co_await repo::admin_sessions::Update(s);
    }
  }
}
