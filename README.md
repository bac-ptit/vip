# VIP Backend - High-Performance C++ Web System

## Overview
VIP Backend is a robust, high-performance web service built with modern C++ features. It leverages the **Drogon** web framework to provide a scalable and efficient infrastructure for managing complex business data including products, administrative sessions, and project content.

The system is designed with **C++26** standards, emphasizing performance through asynchronous coroutines, in-memory caching, and extremely fast JSON serialization via **Glaze**.

## Technology Stack
- **Language:** C++26 (ISO Standard)
- **Web Framework:** [Drogon](https://drogon.org/) (Coroutines-based, high performance)
- **Serialization:** [Glaze](https://github.com/stephenberry/glaze) (Mandatory for all DTOs)
- **Database:** PostgreSQL (managed through Drogon ORM)
- **Security:** `libsodium` (Hashing & Encryption)
- **Build System:** CMake (with C++ Modules support)
- **Package Manager:** `vcpkg` (Manifest mode)

## Architecture
The project follows a clean, multi-layered architectural pattern to ensure separation of concerns and maintainability:

1.  **Controllers:** Handle HTTP routing and request parsing.
2.  **Services:** Orchestrate business logic and manage cross-repository operations.
3.  **Repositories:** Abstract database access using `drogon::orm::CoroMapper`.
4.  **Domains:** Business entities wrapping generated database models for logic and validation.
5.  **DTOs (Data Transfer Objects):** Plain structs for efficient JSON serialization/deserialization.
6.  **Models:** Auto-generated database models (DO NOT EDIT).

## Key Features
- **Modern C++ Modules:** Core logic is organized into `.cppm` modules for faster compilation and better encapsulation.
- **Asynchronous Operations:** Fully non-blocking execution using `drogon::Task<>` and `co_await`.
- **In-Memory Caching:** Products and related assets are cached in memory for sub-millisecond response times.
- **Global Auth Filtering:** Dynamic authentication with flexible exemptions defined in `config.yaml`.
- **Prometheus Metrics:** Integrated metrics exporter for monitoring.

## Getting Started

### Prerequisites
- CMake (>= 3.5)
- vcpkg
- PostgreSQL
- A compiler with C++26 and Modules support (e.g., GCC 14+, Clang 18+)

### Building
1. **Install dependencies:**
   ```bash
   vcpkg install
   ```

2. **Configure and Build:**
   ```bash
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
   cmake --build build
   ```

### Configuration
Modify `config.yaml` to set your database credentials, server port, and authentication exemptions.

```yaml
db_clients:
  - name: default
    rdbms: postgresql
    host: 127.0.0.1
    dbname: qlattt
    user: 'your_user'
    passwd: 'your_password'
```

## Project Structure
- `controllers/`: HTTP endpoint definitions.
- `services/`: Business logic implementations.
- `repositories/`: Database access layers.
- `domains/`: Business entity wrappers.
- `dto/`: Request/Response data structures.
- `models/`: Auto-generated ORM models.
- `filters/`: Middleware (e.g., `AuthFilter`).
- `utils/`: Common utilities (e.g., `PasswordHasher`).

## Coding Standards
- Use **Brace Initialization** `{}` globally.
- Prefer **C++ Modules** (`.cppm`) over traditional headers.
- All asynchronous handlers must return `drogon::Task<>`.
- Use `gsl::` for standard library safety.
- **Models** are read-only; implement logic in the Domain layer.

## Testing
Run the test suite using `ctest`:
```bash
cd build
ctest
```

---
*Built with ❤️ using Modern C++*
