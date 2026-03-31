# VIP Project Backend: High-Performance Real Estate Platform

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-26-blue.svg)](https://en.cppreference.com/w/cpp/compiler_support)
[![Framework](https://img.shields.io/badge/Framework-Drogon-red.svg)](https://drogon.org)
[![Serialization](https://img.shields.io/badge/Serialization-Glaze-green.svg)](https://github.com/stephenberry/glaze)

## 🚀 Overview

The **VIP Backend** is a cutting-edge, high-concurrency system designed for premium real estate management. Built with **C++26**, it leverages modern paradigms such as **Coroutines** and **C++20 Modules** to deliver exceptional throughput and minimal latency.

Originally designed with Google Cloud Storage (GCS) integration, the system has been refactored to use **ImgBB** via a custom-built asynchronous plugin. This transition significantly reduces overhead while maintaining high reliability for image assets.

## ✨ Core Technical Highlights

*   **Modern C++26 Stack**: Fully utilizes C++20 Modules for strict encapsulation and faster compilation, alongside Coroutines for non-blocking I/O.
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
git clone https://github.com/your-repo/vip-backend.git
cd vip-backend

# Build and start the infrastructure (Backend + Postgres)
docker-compose up --build -d
```

### Build Optimization
The project utilizes **CMake Presets** and a custom **vcpkg triplet** (`x64-linux-release`) to enforce release-only builds, drastically reducing image size and build times.

## 📖 API Documentation

Comprehensive API specifications are available via OpenAPI 3.0. Once the server is running, access the documentation at:
`http://localhost:5556/docs/index.html`

## 🏗 Coding Standards

*   **Google C++ Style Guide**: Adheres to naming conventions (`snake_case`, `trailing_underscore_`) and formatting rules.
*   **Safety First**: Extensive use of `ms-gsl` (Guidelines Support Library) concepts and C++ attributes (`[[nodiscard]]`, `[[maybe_unused]]`) for robust code.
*   **Async Everything**: All service and repository layers are designed with `drogon::Task<T>` to maximize CPU utilization.

---
*Maintained with excellence by the VIP Engineering Team.*
