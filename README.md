# VIP Backend

High-performance C++ backend for the VIP real estate platform, built with Drogon, Glaze, PostgreSQL, and libsodium.

## Overview

VIP Backend is a layered C++ web service focused on fast request handling, async database access, and predictable domain boundaries. The codebase follows a Controller -> Service -> Repository -> Domain -> DTO structure and uses Drogon coroutines for non-blocking workflows.

The project uses **Pixi as the main development package manager**. Pixi provides the compiler/tooling environment, while CMake and vcpkg resolve and build the C++ dependencies declared in `vcpkg.json`.

## Tech Stack

| Tool or library | Purpose |
| --- | --- |
| Pixi | Development environment and task runner |
| CMake Presets | Reproducible configure/build profiles |
| Ninja | Fast build backend |
| vcpkg | C++ dependency resolution through manifest mode |
| Clang >= 21 | Primary C++ compiler |
| Drogon | HTTP framework and ORM |
| Glaze | High-performance JSON serialization |
| yaml-cpp | YAML configuration support |
| libsodium | Password hashing and security primitives |
| PostgreSQL | Primary relational database |
| Docker Compose | Containerized database and backend runtime |

## Requirements

- Pixi
- Docker and Docker Compose
- Clang >= 21, provided by Pixi for local builds
- On Linux with Clang + libstdc++, libstdc++ >= 15

Install Pixi from the official installer if it is not already available:

```bash
curl -fsSL https://pixi.sh/install.sh | sh
```

## Quick Start With Pixi

Install the Pixi environment:

```bash
pixi install
```

Start PostgreSQL with Docker Compose:

```bash
docker compose up -d db
```

Build and run the debug binary:

```bash
pixi run run
```

The backend listens on:

```text
http://localhost:5555
```

API documentation is served from:

```text
http://localhost:5555/docs/index.html
```

## Pixi Tasks

| Command | Description |
| --- | --- |
| `pixi run ensure-vcpkg` | Prepare the local `.vcpkg` checkout used by CMake presets |
| `pixi run build` | Configure and build the default debug preset |
| `pixi run run` | Build and run the debug binary |
| `pixi run build-debug` | Explicit debug build |
| `pixi run run-debug` | Explicit debug run |
| `pixi run build-release` | Configure and build the local release preset |
| `pixi run run-release` | Build and run the local release binary |
| `pixi run configure` | Fresh configure for the debug preset |
| `pixi run configure-release` | Fresh configure for the release preset |
| `pixi run clean` | Remove local debug and release build directories |
| `pixi run fmt` | Format C++ sources with clang-format |
| `pixi run run-docker` | Build and run the full Docker Compose stack |

Local debug artifacts are written to `cmake-build-debug/`.
Local release artifacts are written to `cmake-build-release/`.

## Docker Usage

Run the full stack with Pixi:

```bash
pixi run run-docker
```

This runs:

```bash
docker compose up --build
```

The Compose stack starts:

- `vip-postgres` on port `5432`
- `vip-backend` on port `5555`

Stop the stack with:

```bash
docker compose down
```

The Dockerfile uses a multi-stage build. Pixi is used only in the builder stage to install the build environment and run the CMake/vcpkg build. The final runtime image contains only the runtime dependencies, configuration, static files, and compiled `vip` binary.

## Configuration

The application loads configuration from:

```text
config.yaml
```

For local Pixi runs, the default database host is `127.0.0.1`, which works with:

```bash
docker compose up -d db
```

During Docker image build, the Dockerfile rewrites the database host to `db` so the app container can reach the Compose PostgreSQL service.

## Dependency Management

C++ dependencies are declared in `vcpkg.json`, including:

- Drogon with `ctl`, `postgres`, and `yaml` features
- Glaze
- libsodium
- yaml-cpp

CMake uses vcpkg manifest mode through the configured presets. You usually do not need to call vcpkg directly; use Pixi tasks instead.

## Architecture Notes

- Controllers handle HTTP routing and request/response boundaries.
- Services own business workflows.
- Repositories own database access.
- Domains and DTOs define API and business data shapes.
- Generated model files live under `models/`.

The project currently builds with the configured CMake standard in `CMakeLists.txt` and requires Clang >= 21 for the primary local toolchain.
