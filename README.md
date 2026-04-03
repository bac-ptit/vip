# VIP Project Backend: High-Performance Real Estate Platform

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-26-blue.svg)](https://en.cppreference.com/w/cpp/compiler_support)
[![Framework](https://img.shields.io/badge/Framework-Drogon-red.svg)](https://drogon.org)
[![Serialization](https://img.shields.io/badge/Serialization-Glaze-green.svg)](https://github.com/stephenberry/glaze)

## 🚀 Overview

The **VIP Backend** is a cutting-edge, high-concurrency system designed for premium real estate management. Built with **C++26**, it leverages modern paradigms such as **Coroutines** to deliver exceptional throughput and minimal latency.

> **Note on Architecture**: While the project was initially intended to use **C++20 Modules**, the implementation has been transitioned back to **Traditional Headers**. This decision was made to optimize memory consumption during the build process, as C++ Module compilation proved to be excessively memory-intensive for the current infrastructure.

## ✨ Core Technical Highlights

*   **Modern C++26 Stack**: Fully utilizes **Coroutines** for non-blocking I/O and modern language features for performance and safety.
*   **Extreme Cache Performance**: Implements a high-performance in-memory cache strategy. Responses for critical entities (like `Products`) are stored as **pre-serialized JSON strings** using [Glaze](https://github.com/stephenberry/glaze), bypassing redundant serialization cycles during request handling.
*   **Reactive Image Handling**: Integrated `ImgBBPlugin` using `drogon::HttpClient` for seamless, non-blocking image uploads.
*   **Layered Architecture**: Strictly follows the **Controller → Service → Repository → Domain → DTO** pattern, ensuring clean separation of concerns and robust maintainability.
*   **High-Speed Serialization**: Powered by [Glaze](https://github.com/stephenberry/glaze), the fastest JSON library for modern C++, providing automatic reflection-based serialization.

## 🛠 Tech Stack & Dependencies

| Library | Purpose | Link |
| :--- | :--- | :--- |
| **Drogon** | Core Web Framework (C++20/26) | [GitHub](https://github.com/drogonframework/drogon) |
| **Glaze** | High-performance JSON Serialization | [GitHub](https://github.com/stephenberry/glaze) |
| **libsodium** | Cryptographic Hashing & Security | [Link](https://libsodium.org) |
| **PostgreSQL** | Primary Relational Database | [Link](https://www.postgresql.org) |
| **vcpkg** | C++ Dependency Management | [Link](https://vcpkg.io) |

## 📦 Deployment & Environment

The system is fully containerized using **Fedora 43** as the base image for both build and runtime stages, ensuring a modern GLIBC and compiler environment.

### Prerequisites
*   **Docker & Docker Compose**
*   **ImgBB API Key** (Configured in `config.yaml`)

### Quick Start
```bash
# Clone the repository
git clone https://github.com/bac-ptit/vip.git
cd vip-backend

# Build and start the infrastructure (Backend + Postgres)
docker-compose up --build -d
```

### Build Optimization
The project utilizes **CMake Presets**, **Ninja** build system, and a custom **vcpkg triplet** (`x64-linux-release`) to enforce release-only builds, drastically reducing image size and build times.

## 📖 API Documentation

Comprehensive API specifications are available via OpenAPI 3.0. Once the server is running, access the documentation at:
`http://localhost:5556/docs/index.html`

## 🏗 Coding Standards

*   **Google C++ Style Guide**: Adheres to naming conventions (`snake_case`) and formatting rules.
*   **Modern Initialization**: Extensive use of **Brace Initialization** (`{}`) consistently across the codebase for safer and predictable object creation.
*   **Safety First**: Extensive use of C++ attributes (`[[nodiscard]]`, `[[maybe_unused]]`) for robust code.
*   **Async Everything**: All service and repository layers are designed with `drogon::Task<T>` to maximize CPU utilization.

---
*Maintained with excellence by the VIP Engineering Team.*
