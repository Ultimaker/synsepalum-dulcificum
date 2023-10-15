cmake_minimum_required(VERSION 3.23)
project(dulcificum)

find_package(standardprojectsettings REQUIRED)
option(EXTENSIVE_WARNINGS "Build with all warnings" ON)
option(ENABLE_TESTS "Build with unit test" ON)
option(WITH_APPS "Build with apps" ON)
if(WITH_APPS)
    set(APP_VERSION "0.1.0" CACHE STRING "Version of the translator app" )
    message(STATUS "Configuring translator app version: ${APP_VERSION}")
endif ()

option(WITH_PYTHON_BINDINGS "Build with Python bindings: `pyDulcificum`" ON)
if(WITH_PYTHON_BINDINGS)
    set(PYDULCIFICUM_VERSION "0.1.0" CACHE STRING "Version of the pyDulcificum python bindings" )
    message(STATUS "Configuring pyDulcificum version: ${PYDULCIFICUM_VERSION}")
endif ()

set(DULCIFICUM_VERSION "0.1.0" CACHE STRING "Version of the dulcificum library" )
message(STATUS "Configuring Dulcificum version: ${DULCIFICUM_VERSION}")

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)

# --- Setup the shared C++ mgjtp library ---
set(DULCIFICUM_SRC
        src/command_types.cpp
        src/miracle_jtp/mgjtp_command_to_json.cpp
        src/miracle_jtp/mgjtp_json_to_command.cpp
)
add_library(dulcificum ${DULCIFICUM_SRC})
target_link_libraries(dulcificum
        PUBLIC
        nlohmann_json::nlohmann_json
        PRIVATE
        range-v3::range-v3
        spdlog::spdlog)

target_include_directories(dulcificum
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(dulcificum PRIVATE DULCIFICUM_VERSION="${DULCIFICUM_VERSION}")

use_threads(dulcificum)
enable_sanitizers(dulcificum)
if (${EXTENSIVE_WARNINGS})
    set_project_warnings(dulcificum)
endif ()

# --- Setup Python bindings ---
if (WITH_PYTHON_BINDINGS)
    find_package(pybind11 REQUIRED)
    find_package(cpython REQUIRED)

    pybind11_add_module(pyDulcificum pyDulcificum/pyDulcificum.cpp)
    target_link_libraries(pyDulcificum PUBLIC dulcificum pybind11::pybind11 cpython::cpython)
    target_compile_definitions(pyDulcificum PRIVATE PYDULCIFICUM_VERSION="${PYDULCIFICUM_VERSION}")
    if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
        pybind11_strip(pyDulcificum)
    endif()
endif ()

# --- Setup tests ---
if (ENABLE_TESTS)
    add_subdirectory(test)
endif ()

# --- Setup Apps ---
if (WITH_APPS)
    add_subdirectory(apps)
endif ()
