set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_CMAKE_SYSTEM_NAME Linux)

# Chỉ build phiên bản Release, bỏ qua Debug để tiết kiệm dung lượng và thời gian
set(VCPKG_BUILD_TYPE release)
