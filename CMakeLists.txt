cmake_minimum_required(VERSION 3.30)

project(vip CXX)

include(CheckIncludeFileCXX)
include(CheckCXXSourceCompiles)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 21)
                message(FATAL_ERROR "Clang >= 21 is required, found ${CMAKE_CXX_COMPILER_VERSION}.")
        endif ()
elseif (MSVC)
        message(STATUS "MSVC detected; skipping Clang/libstdc++ version guards.")
else ()
        message(FATAL_ERROR "This project expects Clang >= 21 or MSVC, found ${CMAKE_CXX_COMPILER_ID}.")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
        check_cxx_source_compiles("
                #include <string>
                #ifndef __GLIBCXX__
                #error not using libstdc++
                #endif
                int main() { return 0; }
        " VIP_USES_LIBSTDCXX)

        if (VIP_USES_LIBSTDCXX)
                check_cxx_source_compiles("
                        #include <bits/c++config.h>
                        #if !defined(_GLIBCXX_RELEASE) || _GLIBCXX_RELEASE < 15
                        #error libstdc++ 15 or newer is required
                        #endif
                        int main() { return 0; }
                " VIP_LIBSTDCXX_AT_LEAST_15)

                if (NOT VIP_LIBSTDCXX_AT_LEAST_15)
                        message(FATAL_ERROR "Clang with libstdc++ requires libstdc++ >= 15.")
                endif ()
        else ()
                message(STATUS "Clang is not using libstdc++; skipping libstdc++ version guard.")
        endif ()
endif ()

# Clang's libc++ hides deprecated codecvt facets used by generated model code.
# Re-enable them to keep current codegen compatible.
add_compile_definitions(_LIBCPP_ENABLE_CXX17_REMOVED_CODECVT)

add_executable(${PROJECT_NAME} main.cc
        domains/registrations.cc)

# Generated model files use std::wstring_convert/std::codecvt_utf8_utf16 but do
# not include the required headers; force-include them at compile time.
if (NOT MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE "SHELL:-include locale" "SHELL:-include codecvt")
endif ()

# Find clang-tidy
# find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

# ##############################################################################
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial-sodium::sodium)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp::yaml-cpp)

# ##############################################################################

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(domains DOMAIN_SRC)
aux_source_directory(utils UTILS_SRC)
aux_source_directory(repositories REPO_SRC)
aux_source_directory(services SERVICE_SRC)
aux_source_directory(dto DTO_SRC)

# Collect all sources that SHOULD be checked by clang-tidy
set(TIDY_SRC 
    main.cc
    ${CTL_SRC}
    ${FILTER_SRC}
    ${PLUGIN_SRC}
    ${DOMAIN_SRC}
    ${UTILS_SRC}
    ${REPO_SRC}
    ${SERVICE_SRC}
    ${DTO_SRC}
    plugins/ImgBBPlugin.cc
)

# if(CLANG_TIDY_EXE)
#     # Set clang-tidy only for non-model sources
#     # Use -- to separate tidy flags from compiler flags, and try to ignore module flags
#     set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE};--extra-arg=-Wno-unknown-warning-option;--extra-arg=-fno-modules-ts")
#     set_source_files_properties(${TIDY_SRC} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
# endif()

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
        ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/models)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${CTL_SRC}
        ${FILTER_SRC}
        ${PLUGIN_SRC}
        ${MODEL_SRC}
        ${DOMAIN_SRC}
        ${UTILS_SRC}
        ${REPO_SRC}
        ${SERVICE_SRC}
        ${DTO_SRC}
        plugins/ImgBBPlugin.cc)

add_custom_target(run-vip
        COMMAND $<TARGET_FILE:${PROJECT_NAME}>
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>
        DEPENDS ${PROJECT_NAME}
        USES_TERMINAL
        VERBATIM)

# ##############################################################################
add_subdirectory(test)
