# Stage 1: Build stage
FROM fedora:43 AS builder

# Tối ưu hóa DNF
RUN echo "max_parallel_downloads=10" >> /etc/dnf/dnf.conf && \
    echo "fastestmirror=True" >> /etc/dnf/dnf.conf

# Cài đặt công cụ build và dependencies của Drogon (Loại bỏ cmake của Fedora)
# Fallback cũ nếu một port Fedora/vcpkg cần GCC:
#   gcc
#   gcc-c++
RUN --mount=type=cache,target=/var/cache/dnf \
    dnf install -y \
    clang \
    ninja-build \
    git \
    awk \
    curl \
    wget \
    zip \
    unzip \
    tar \
    pkg-config \
    autoconf \
    autoconf-archive \
    automake \
    libtool \
    libstdc++-devel \
    glibc-devel \
    openssl-devel \
    uuid-devel \
    libpq-devel \
    libsodium-devel \
    yaml-cpp-devel \
    jsoncpp-devel \
    zlib-devel \
    kernel-headers \
    perl \
    perl-core \
    make \
    && dnf clean all

# --- CÀI ĐẶT PIXI ---
ENV PIXI_HOME=/opt/pixi
ENV PATH="${PIXI_HOME}/bin:${PATH}"
RUN curl -fsSL https://pixi.sh/install.sh | PIXI_NO_PATH_UPDATE=1 sh

# --- CÀI ĐẶT CMAKE 4.2.1 TỪ KITWARE (cũ; Pixi cung cấp CMake) ---
# WORKDIR /tmp
# RUN wget https://github.com/Kitware/CMake/releases/download/v4.2.1/cmake-4.2.1-linux-x86_64.sh && \
#     chmod +x cmake-4.2.1-linux-x86_64.sh && \
#     ./cmake-4.2.1-linux-x86_64.sh --skip-license --prefix=/usr/local && \
#     rm cmake-4.2.1-linux-x86_64.sh

# Thiết lập Compiler là Clang (Quan trọng để build C++20/26 Modules)
ENV CC=clang
ENV CXX=clang++

# --- CÀI ĐẶT DROGON (cũ; vcpkg.json quản lý Drogon + yaml + postgres) ---
# WORKDIR /tmp
# RUN git clone --depth 1 https://github.com/drogonframework/drogon && \
#     cd drogon && \
#     git submodule update --init && \
#     mkdir build && cd build && \
#     cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release && \
#     ninja install && \
#     cd /tmp && rm -rf drogon

# --- CÀI ĐẶT VCPKG ---
WORKDIR /opt
RUN git clone https://github.com/microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh -disableMetrics

ENV VCPKG_ROOT=/opt/vcpkg

# Copy project files
WORKDIR /app
COPY . .

# Thay đổi cấu hình DB để trỏ vào container "db"
RUN sed -i 's/host: 127.0.0.1/host: db/' config.yaml

# Cài môi trường Pixi từ lockfile trước khi build
RUN --mount=type=cache,target=/root/.cache/rattler \
    pixi install --frozen

# Build project với cache cho vcpkg_installed để lần build sau nhanh hơn
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
# Lệnh cũ nếu cần quay lại:
# RUN --mount=type=cache,target=/app/build/vcpkg_installed \
#     cmake --preset docker-release && \
#     cmake --build --preset docker-release
RUN --mount=type=cache,target=/app/build/vcpkg_installed \
    pixi run build-docker

# Stage 2: Runtime stage
FROM fedora:43

RUN echo "max_parallel_downloads=10" >> /etc/dnf/dnf.conf

# Cài đặt runtime dependencies (không có -devel)
RUN --mount=type=cache,target=/var/cache/dnf \
    dnf install -y \
    libstdc++ \
    openssl \
    uuid \
    libpq \
    libsodium \
    yaml-cpp \
    jsoncpp \
    zlib \
    && dnf clean all

# Thiết lập thư mục làm việc
WORKDIR /app

# Copy các tệp cấu hình và dữ liệu tĩnh
COPY --from=builder /app/config.yaml .
COPY --from=builder /app/public ./public

# Tạo thư mục bin và copy binary từ stage builder
RUN mkdir -p /app/bin
COPY --from=builder /app/build/vip /app/bin/vip

EXPOSE 5555

# Chuyển vào thư mục bin và chạy để code load "../config.yaml" chính xác
WORKDIR /app/bin
CMD ["./vip"]
