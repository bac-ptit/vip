//
// Created by bac on 3/27/26.
//
module;
#include <drogon/drogon.h>
#include <models/Admins.h>
#include <atomic>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

module service;

import domain;
import repo;
import dto;

using namespace drogon;

// Helper function to get one of the last 2 threads for CPU-intensive auth operations
static trantor::EventLoop* GetNextAuthLoop() noexcept {
  static std::atomic<size_t> counter{0};
  const size_t total_threads{drogon::app().getThreadNum()};
  
  if (total_threads < 2) [[unlikely]] {
    return drogon::app().getIOLoop(0);
  }
  
  const size_t base_index{total_threads - 2};
  const size_t target_index{base_index + (counter.fetch_add(1, std::memory_order_relaxed) % 2)};
  return drogon::app().getIOLoop(target_index);
}

// Helper function to convert domain::Admin to dto::AdminResponse
static dto::AdminResponse ToAdminResponse(const domain::Admin& admin) {
  dto::AdminResponse response;
  response.id = admin.getId();
  response.username = admin.getUsername();
  response.full_name = admin.getFullName();
  response.is_active = admin.getIsActive();
  response.created_at = admin.getCreatedAt();
  response.updated_at = admin.getUpdatedAt();
  
  return response;
}

drogon::Task<dto::AdminResponse> service::admin::Register(
    dto::RegisterAdminRequest request) {
  // Check if username is already taken
  auto exists{co_await repo::admin::ExistsByUsername(request.username)};
  if (exists) {
    throw std::runtime_error("Username already exists");
  }

  // Switch to dedicated auth thread for CPU-intensive password hashing
  co_await drogon::switchThreadCoro(GetNextAuthLoop());

  // Create new admin with validation - move fields
  domain::Admin admin;
  admin.SetUsername(std::move(request.username));
  admin.SetPassword(request.password);  // Password needs copy (hashing)
  admin.SetFullName(std::move(request.full_name));
  admin.setIsActive(true);

  // Create in database
  auto created{co_await repo::admin::Create(admin)};
  co_return ToAdminResponse(created);
}

drogon::Task<std::optional<dto::AdminResponse>> service::admin::Login(
    dto::LoginRequest&& request) {
  // Find admin by username
  auto password = std::move(request.password);
  auto admin{co_await repo::admin::FindByUsername(request.username)};
  if (!admin) {
    co_return std::nullopt;
  }

  // Check if admin is active
  if (!admin->getValueOfIsActive()) {
    throw std::runtime_error("Account is deactivated");
  }

  // Switch to dedicated auth thread for CPU-intensive password verification
  co_await drogon::switchThreadCoro(GetNextAuthLoop());

  // Verify password using domain method
  if (!admin->VerifyPassword(password)) {
    co_return std::nullopt;
  }

  co_return ToAdminResponse(*admin);
}

drogon::Task<void> service::admin::UpdateProfile(
    std::string_view id, dto::UpdateProfileRequest request) {
  // Get existing admin
  auto admin{co_await repo::admin::FindById(id)};
  if (!admin) {
    throw std::runtime_error("Admin not found");
  }

  // Update username if provided
  if (request.username.has_value()) {
    // Check if new username is taken by another admin
    auto existing{co_await repo::admin::FindByUsername(*request.username)};
    if (existing && existing->getValueOfId() != id) {
      throw std::runtime_error("Username already exists");
    }
    admin->SetUsername(std::move(*request.username));
  }

  // Update full name if provided
  if (request.full_name.has_value()) {
    admin->SetFullName(std::move(*request.full_name));
  }

  // Update in database
  co_await repo::admin::Update(*admin);
}

drogon::Task<void> service::admin::ChangePassword(
    std::string_view id, dto::ChangePasswordRequest request) {
  // Get existing admin
  auto admin{co_await repo::admin::FindById(id)};
  if (!admin) {
    throw std::runtime_error("Admin not found");
  }

  // Verify old password using domain method
  if (!admin->VerifyPassword(request.old_password)) {
    throw std::runtime_error("Current password is incorrect");
  }

  // Set new password (will validate and hash) - password needs copy for hashing
  admin->SetPassword(request.new_password);

  // Update in database
  co_await repo::admin::Update(*admin);
}

drogon::Task<void> service::admin::Deactivate(std::string_view id) {
  auto success{co_await repo::admin::SoftDeleteById(id)};
  if (!success) {
    throw std::runtime_error("Admin not found");
  }
}

drogon::Task<void> service::admin::Activate(std::string_view id) {
  auto admin{co_await repo::admin::FindById(id)};
  if (!admin) {
    throw std::runtime_error("Admin not found");
  }

  admin->setIsActive(true);
  co_await repo::admin::Update(*admin);
}

drogon::Task<std::optional<dto::AdminResponse>> service::admin::GetById(
    std::string_view id) {
  auto admin{co_await repo::admin::FindById(id)};
  if (!admin) {
    co_return std::nullopt;
  }
  co_return ToAdminResponse(*admin);
}

drogon::Task<std::optional<dto::AdminResponse>> service::admin::GetByUsername(
    std::string_view username) {
  auto admin{co_await repo::admin::FindByUsername(username)};
  if (!admin) {
    co_return std::nullopt;
  }
  co_return ToAdminResponse(*admin);
}

drogon::Task<std::vector<dto::AdminResponse>> service::admin::GetAll() {
  auto admins{co_await repo::admin::FindAll()};
  std::vector<dto::AdminResponse> responses;
  responses.reserve(admins.size());
  
  for (const auto& admin : admins) {
    responses.push_back(ToAdminResponse(admin));
  }
  
  co_return responses;
}

drogon::Task<std::vector<dto::AdminResponse>> service::admin::GetActiveAdmins() {
  auto admins{co_await repo::admin::FindActiveAdmins()};
  std::vector<dto::AdminResponse> responses;
  responses.reserve(admins.size());
  
  for (const auto& admin : admins) {
    responses.push_back(ToAdminResponse(admin));
  }
  
  co_return responses;
}

drogon::Task<bool> service::admin::IsUsernameAvailable(
    std::string_view username) {
  auto exists{co_await repo::admin::ExistsByUsername(username)};
  co_return !exists;
}
