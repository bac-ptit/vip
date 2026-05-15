cmake_minimum_required(VERSION 3.20)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(VCPKG_DIR "${PROJECT_ROOT}/.vcpkg")

if(NOT EXISTS "${VCPKG_DIR}/.vcpkg-root")
    if(EXISTS "${VCPKG_DIR}")
        message(FATAL_ERROR
            "${VCPKG_DIR} exists but is not a valid vcpkg checkout. "
            "Remove it and run the Pixi task again.")
    endif()

    find_program(GIT_EXECUTABLE git REQUIRED)
    message(STATUS "Cloning vcpkg into ${VCPKG_DIR}")
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone https://github.com/microsoft/vcpkg.git "${VCPKG_DIR}"
        RESULT_VARIABLE VCPKG_CLONE_RESULT)

    if(NOT VCPKG_CLONE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to clone vcpkg into ${VCPKG_DIR}.")
    endif()
endif()

if(CMAKE_HOST_WIN32)
    set(VCPKG_BINARY "${VCPKG_DIR}/vcpkg.exe")
    set(VCPKG_BOOTSTRAP_COMMAND cmd /c "${VCPKG_DIR}/bootstrap-vcpkg.bat" -disableMetrics)
else()
    set(VCPKG_BINARY "${VCPKG_DIR}/vcpkg")
    set(VCPKG_BOOTSTRAP_COMMAND "${VCPKG_DIR}/bootstrap-vcpkg.sh" -disableMetrics)
endif()

if(NOT EXISTS "${VCPKG_BINARY}")
    message(STATUS "Bootstrapping vcpkg in ${VCPKG_DIR}")
    execute_process(
        COMMAND ${VCPKG_BOOTSTRAP_COMMAND}
        WORKING_DIRECTORY "${VCPKG_DIR}"
        RESULT_VARIABLE VCPKG_BOOTSTRAP_RESULT)

    if(NOT VCPKG_BOOTSTRAP_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to bootstrap vcpkg in ${VCPKG_DIR}.")
    endif()
endif()

message(STATUS "vcpkg is ready at ${VCPKG_DIR}")
