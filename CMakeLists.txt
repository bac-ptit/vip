cmake_minimum_required(VERSION 3.30)

project(vip CXX)

include(CheckIncludeFileCXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(${PROJECT_NAME} main.cc)

# Find clang-tidy
# find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

# ##############################################################################
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial-sodium::sodium)

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

# ##############################################################################
add_subdirectory(test)
