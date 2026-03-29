//
// Created by bac on 3/29/26.
//
module;
#include <drogon/drogon.h>
#include <models/ActivityLogs.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

module service;

import domain;
import repo;
import dto;

using namespace drogon;

static dto::ActivityLogResponse ToActivityLogResponse(const domain::ActivityLogs& log) {
  dto::ActivityLogResponse response;
  response.id = log.getId();
  response.admin_id = log.getAdminId();
  response.action = log.getAction();
  response.entity = log.getEntity();
  response.entity_id = log.getEntityId();
  response.detail = log.getDetail();
  response.ip_address = log.getIpAddress();
  response.created_at = log.getCreatedAt();
  return response;
}

drogon::Task<std::vector<dto::ActivityLogResponse>> service::activity_logs::GetAll() {
  auto logs{co_await repo::activity_logs::FindAll()};
  std::vector<dto::ActivityLogResponse> responses;
  responses.reserve(logs.size());
  for (auto&& l : logs) {
    responses.push_back(ToActivityLogResponse(l));
  }
  co_return responses;
}

drogon::Task<std::vector<dto::ActivityLogResponse>> service::activity_logs::GetByAdminId(const std::string& adminId) {
  auto logs{co_await repo::activity_logs::FindByAdminId(adminId)};
  std::vector<dto::ActivityLogResponse> responses;
  responses.reserve(logs.size());
  for (auto&& l : logs) {
    responses.push_back(ToActivityLogResponse(l));
  }
  co_return responses;
}

drogon::Task<void> service::activity_logs::LogAction(const std::string& adminId, const std::string& action, const std::string& entity, const std::string& entityId, const std::string& detail, const std::string& ipAddress) {
  domain::ActivityLogs activityLog;
  if (!adminId.empty()) {
    activityLog.setAdminId(adminId);
  }
  activityLog.setAction(action);
  if (!entity.empty()) {
    activityLog.setEntity(entity);
  }
  if (!entityId.empty()) {
    activityLog.setEntityId(entityId);
  }
  if (!detail.empty()) {
    activityLog.setDetail(detail);
  }
  if (!ipAddress.empty()) {
    activityLog.setIpAddress(ipAddress);
  }
  
  co_await repo::activity_logs::Create(activityLog);
}
