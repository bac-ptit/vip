---
name: vip-backend-developer
description: High-performance C++26 development for the VIP backend project using Drogon and Glaze. Use this skill for implementing domains, repositories, services, controllers, and filters, ensuring architectural consistency, memory caching, and model immutability.
---

# VIP Backend Development Guidelines

This skill provides expert procedural knowledge for completing the VIP backend project based on established database
models and C++20 standards.

## Core Mandates

1. **Immutability of Models**: The `models/` directory is strictly **READ-ONLY**. generated models must NOT be modified.
   Business logic, data validation, and transformations must be implemented in the `domain/`, `dto/`, or `service/`
   layers.
2. **Dependency Management**: All third-party libraries (e.g., `glaze`, `libsodium`) must be managed exclusively through
   `vcpkg.json`.
3. **High-Performance Serialization**: `glaze` is the mandatory library for all JSON serialization and deserialization.
   `JsonCpp` is prohibited for new implementations.
4. **Modern C++26 Conventions**:
    * **Modules**: Use C++ Modules (`.cppm`) for all new architectural layers.
    * **Brace Initialization**: Use brace initialization `{}` globally (e.g., `auto response{co_await ...};`).
    * **Concise Initializers**: Omit empty braces `{}` for default construction (e.g., use `std::string username;`
      instead of `std::string username{};`).
    * **Coroutines**: Utilize `drogon::Task<>` for all asynchronous operations in Controllers, Services, and
      Repositories.
    * **Attributes**: Use attribute for if, switch, and loop
      statements, [[nodiscard]], [[maybe_unused]], [[deprecated]],... or performance optimization.
    * **C++ Features**: Use std::to_underlying, std::ranges, std::views, noexcept, const whenenver possbile, std::
      unreachable for readability and performance.
    * **Cpp Core Guidelines**: Use `gsl::` for all standard library usage.
    * **Coroutines Argument**: 
      - Passing Arguments: Always pass by value in the coroutine signature if the argument is
        needed after a co_await point.
      - Performance Optimization: To avoid unnecessary copies, the caller should use std::move() to transfer ownership of the
      object directly into the coroutine frame.
      - Granularity: 
        * Small Objects: Pass the entire object by value.
        * Large Objects/Few Fields: If only a few fields are required from a large object, you may pass the object by reference
        but must capture the necessary fields into local variables before the first co_await.

5. **Dynamic Configuration**: Utilize `config.yaml` for custom logic (e.g., path exemptions in `AuthFilter`). Retrieve
   config using `drogon::app().getCustomConfig()`.
6. Third-party libraries should be installed using `vcpkg` in manifest mode, I suggest some third-party libraries I
   usually use:
    * `fmt`
    * `glaze`
    * `boost`
    * `abseil`

## Layered Implementation Workflow

### 1. Data Transfer Objects (DTO) - `dto/*.cppm`

* Define Request and Response structs with **public members**.
* Leverage **Glaze automatic reflection** for serialization; do NOT use `glz::meta` for simple aggregate structs.
* Implement `drogon::fromRequest<T>` specializations for automatic request parsing in controllers.

### 2. Domain Logic - `domains/*.cppm`

* Create wrapper classes (e.g., `domain::Admin`) to encapsulate business logic and validation.
* Inherit from generated `models/` classes only when extending their functionality without modification.

### 3. Repository Layer - `repositories/*.cppm`

* Perform all database operations using `drogon::orm::CoroMapper`.
* Maintain a strict separation between raw database access and business services.

### 4. Service Layer - `services/*.cppm`

* Orchestrate complex business logic and cross-repository operations.
* **In-Memory Caching**: For `Products`, maintain an in-memory cache (`std::unordered_map` or `std::vector`) to maximize
  performance. Update the cache and database concurrently during CRUD operations.
* Implement search algorithms directly on the in-memory cache using modern C++ ranges/algorithms.

### 5. Controller Layer - `controllers/*.h/cc`

* Define routes using `METHOD_ADD` macros.
* All asynchronous handler functions MUST return `Task<>`.
* Leverage DTO arguments directly (e.g., `Task<> Register(dto::RegisterAdminRequest&& request)`).

### 6. Authentication & Middleware - `filters/`

* **AuthFilter**: Implement a `drogon::HttpFilter` that validates sessions.
* **Exemptions**: Respect `auth_exempt_paths` defined in `config.yaml`. Routes for `Products` and `ProductImages` should
  generally bypass authentication unless state-modifying.

## Code Style Exceptions

* **Lowercase Controllers**: Controller classes (e.g., `class admin`) should be lowercase to align with specific project
  mapping and RESTful naming conventions, despite PascalCase being common elsewhere.
* **Main style**: Google C++ style guide is used for consistency.
