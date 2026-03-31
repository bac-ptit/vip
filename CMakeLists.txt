cmake_minimum_required(VERSION 3.5)
project(vip CXX)

include(CheckIncludeFileCXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)


add_executable(${PROJECT_NAME} main.cc)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)
find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial-sodium::sodium)
# find_package(google_cloud_cpp_storage CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE google-cloud-cpp::storage)

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
drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
        ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# use the following line to create views with namespace CHANGE_ME prefixed
# and path namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_sources(${PROJECT_NAME}
        PRIVATE
        ${SRC_DIR}
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

target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET CXX_MODULES

        FILES
        # Utils & Exceptions
        utils/utils.cppm utils/password_hasher.cppm
        exception/custom_exception.cppm
        
        # Aggregators
        domains/domain.cppm
        repositories/repo.cppm
        services/service.cppm
        dto/dto.cppm

        # Admins & Sessions
        domains/admin.cppm repositories/admin_repository.cppm services/admin_service.cppm dto/admin_dto.cppm
        domains/admin_sessions.cppm repositories/admin_sessions_repository.cppm services/admin_sessions_service.cppm dto/admin_sessions_dto.cppm
        
        # Products & Images
        domains/product.cppm repositories/product_repository.cppm services/product_service.cppm dto/product_dto.cppm
        domains/product_images.cppm repositories/product_images_repository.cppm services/product_images_service.cppm dto/product_images_dto.cppm
        
        # Project & Content
        domains/project_info.cppm repositories/project_info_repository.cppm services/project_info_service.cppm dto/project_info_dto.cppm
        domains/media.cppm repositories/media_repository.cppm services/media_service.cppm dto/media_dto.cppm
        domains/floor_plans.cppm repositories/floor_plans_repository.cppm services/floor_plans_service.cppm dto/floor_plans_dto.cppm
        
        # Registrations
        domains/registrations.cppm repositories/registrations_repository.cppm services/registrations_service.cppm dto/registrations_dto.cppm
        domains/registration_interests.cppm repositories/registration_interests_repository.cppm services/registration_interests_service.cppm dto/registration_interests_dto.cppm
        
        # System
        domains/site_settings.cppm repositories/site_settings_repository.cppm services/site_settings_service.cppm dto/site_settings_dto.cppm
        domains/activity_logs.cppm repositories/activity_logs_repository.cppm services/activity_logs_service.cppm dto/activity_logs_dto.cppm
)



# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

add_subdirectory(test)
