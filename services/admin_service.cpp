#include <utility>
#include <stdexcept>
#include <vector>
#include <optional>
#include <string>
#include <ranges>
#include "admin_service.h"

#include <drogon/drogon.h>
#include <models/Admins.h>

#include <domains/domain.h>
#include <repositories/repo.h>
#include <dto/dto.h>

using namespace drogon;

namespace service::admin {

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

drogon::Task<dto::AdminResponse> Register(
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
  auto created{co_await repo::admin::Create(std::move(admin))};
  co_return ToAdminResponse(created);
}

drogon::Task<std::optional<dto::AdminResponse>> Login(
    dto::LoginRequest request) {
  // Find admin by username
  auto password{std::move(request.password)};
  auto admin_opt{co_await repo::admin::FindByUsername(request.username)};
  if (!admin_opt) {
    co_return std::nullopt;
  }

  // Check if admin is active
  if (!admin_opt->getValueOfIsActive()) {
    throw std::runtime_error("Account is deactivated");
  }

  // Switch to dedicated auth thread for CPU-intensive password verification
  co_await drogon::switchThreadCoro(GetNextAuthLoop());

  // Verify password using domain method
  if (!admin_opt->VerifyPassword(password)) {
    co_return std::nullopt;
  }

  co_return ToAdminResponse(*admin_opt);
}

drogon::Task<void> UpdateProfile(
    std::string admin_id, dto::UpdateProfileRequest request) {
  // Get existing admin
  auto admin_opt{co_await repo::admin::FindById(admin_id)};
  if (!admin_opt) {
    throw std::runtime_error("Admin not found");
  }

  // Update username if provided
  if (request.username) {
    // Check if new username is taken by another admin
    auto existing_opt{co_await repo::admin::FindByUsername(*request.username)};
    if (existing_opt && existing_opt->getValueOfId() != admin_id) {
      throw std::runtime_error("Username already exists");
    }
    admin_opt->SetUsername(std::move(*request.username));
  }

  // Update full name if provided
  if (request.full_name) {
    admin_opt->SetFullName(std::move(*request.full_name));
  }

  // Update in database
  co_await repo::admin::Update(*admin_opt);
}

drogon::Task<void> ChangePassword(
    std::string admin_id, dto::ChangePasswordRequest request) {
  // Get existing admin
  auto admin_opt{co_await repo::admin::FindById(admin_id)};
  if (!admin_opt) {
    throw std::runtime_error("Admin not found");
  }

  // Verify old password using domain method
  if (!admin_opt->VerifyPassword(request.old_password)) {
    throw std::runtime_error("Current password is incorrect");
  }

  // Set new password (will validate and hash) - password needs copy for hashing
  admin_opt->SetPassword(request.new_password);

  // Update in database
  co_await repo::admin::Update(*admin_opt);
}

drogon::Task<void> Deactivate(std::string admin_id) {
  auto success{co_await repo::admin::SoftDeleteById(std::move(admin_id))};
  if (!success) {
    throw std::runtime_error("Admin not found");
  }
}

drogon::Task<void> Activate(std::string admin_id) {
  auto admin_opt{co_await repo::admin::FindById(admin_id)};
  if (!admin_opt) {
    throw std::runtime_error("Admin not found");
  }

  admin_opt->setIsActive(true);
  co_await repo::admin::Update(*admin_opt);
}

drogon::Task<std::optional<dto::AdminResponse>> GetById(
    std::string admin_id) {
  auto admin_opt{co_await repo::admin::FindById(std::move(admin_id))};
  if (!admin_opt) {
    co_return std::nullopt;
  }
  co_return ToAdminResponse(*admin_opt);
}

drogon::Task<std::optional<dto::AdminResponse>> GetByUsername(
    std::string username) {
  auto admin_opt{co_await repo::admin::FindByUsername(std::move(username))};
  if (!admin_opt) {
    co_return std::nullopt;
  }
  co_return ToAdminResponse(*admin_opt);
}

drogon::Task<std::vector<dto::AdminResponse>> GetAll() {
  auto admins{co_await repo::admin::FindAll()};
  std::vector<dto::AdminResponse> responses;
  responses.reserve(admins.size());
  
  std::ranges::transform(admins, std::back_inserter(responses), ToAdminResponse);
  
  co_return responses;
}

drogon::Task<std::vector<dto::AdminResponse>> GetActiveAdmins() {
  auto admins{co_await repo::admin::FindActiveAdmins()};
  std::vector<dto::AdminResponse> responses;
  responses.reserve(admins.size());
  
  std::ranges::transform(admins, std::back_inserter(responses), ToAdminResponse);
  
  co_return responses;
}

drogon::Task<bool> IsUsernameAvailable(
    std::string username) {
  auto exists{co_await repo::admin::ExistsByUsername(std::move(username))};
  co_return !exists;
}

}  // namespace service::admin
