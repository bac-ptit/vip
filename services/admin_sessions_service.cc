//
// Created by bac on 3/29/26.
//
module;
#include <drogon/drogon.h>
#include <models/AdminSessions.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

module service;

import domain;
import repo;
import dto;

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

drogon::Task<std::optional<dto::AdminSessionResponse>> service::admin_sessions::GetById(const std::string& id) {
  auto session{co_await repo::admin_sessions::FindById(id)};
  if (!session) {
    co_return std::nullopt;
  }
  co_return ToAdminSessionResponse(*session);
}

drogon::Task<std::optional<dto::AdminSessionResponse>> service::admin_sessions::GetByRefreshToken(const std::string& token) {
  auto session{co_await repo::admin_sessions::FindByRefreshToken(token)};
  if (!session) {
    co_return std::nullopt;
  }
  co_return ToAdminSessionResponse(*session);
}

drogon::Task<std::vector<dto::AdminSessionResponse>> service::admin_sessions::GetByAdminId(const std::string& adminId) {
  auto sessions{co_await repo::admin_sessions::FindByAdminId(adminId)};
  std::vector<dto::AdminSessionResponse> responses;
  responses.reserve(sessions.size());
  for (auto&& s : sessions) {
    responses.push_back(ToAdminSessionResponse(s));
  }
  co_return responses;
}

drogon::Task<void> service::admin_sessions::RevokeSession(const std::string& id) {
  auto session{co_await repo::admin_sessions::FindById(id)};
  if (session) {
    session->setRevoked(true);
    co_await repo::admin_sessions::Update(*session);
  }
}

drogon::Task<void> service::admin_sessions::RevokeAllAdminSessions(const std::string& adminId) {
  auto sessions{co_await repo::admin_sessions::FindByAdminId(adminId)};
  for (auto&& s : sessions) {
    if (!s.getValueOfRevoked()) {
      s.setRevoked(true);
      co_await repo::admin_sessions::Update(s);
    }
  }
}
