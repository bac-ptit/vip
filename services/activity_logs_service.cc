//
// Created by bac on 3/29/26.
//
#include <drogon/drogon.h>
#include <models/ActivityLogs.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include "services/activity_logs_service.h"
#include "domains/domain.h"
#include "repositories/repo.h"
#include "dto/dto.h"

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
  
  std::ranges::transform(logs, std::back_inserter(responses), ToActivityLogResponse);
  
  co_return responses;
}

drogon::Task<std::vector<dto::ActivityLogResponse>> service::activity_logs::GetByAdminId(std::string adminId) {
  auto logs{co_await repo::activity_logs::FindByAdminId(std::move(adminId))};
  std::vector<dto::ActivityLogResponse> responses;
  responses.reserve(logs.size());
  
  std::ranges::transform(logs, std::back_inserter(responses), ToActivityLogResponse);
  
  co_return responses;
}

drogon::Task<void> service::activity_logs::LogAction(std::string adminId, std::string action, std::string entity, std::string entityId, std::string detail, std::string ipAddress) {
  domain::ActivityLogs activityLog;
  if (!adminId.empty()) {
    activityLog.setAdminId(std::move(adminId));
  }
  activityLog.setAction(std::move(action));
  if (!entity.empty()) {
    activityLog.setEntity(std::move(entity));
  }
  if (!entityId.empty()) {
    activityLog.setEntityId(std::move(entityId));
  }
  if (!detail.empty()) {
    activityLog.setDetail(std::move(detail));
  }
  if (!ipAddress.empty()) {
    activityLog.setIpAddress(std::move(ipAddress));
  }
  
  co_await repo::activity_logs::Create(std::move(activityLog));
}
