cmake_minimum_required(VERSION 3.23)
project(dulcificum)

find_package(standardprojectsettings REQUIRED)
option(EXTENSIVE_WARNINGS "Build with all warnings" ON)
option(ENABLE_TESTS "Build with unit test" ON)
option(WITH_APPS "Build with apps" ON)

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

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
        spdlog::spdlog)

target_include_directories(dulcificum
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

use_threads(dulcificum)
enable_sanitizers(dulcificum)
if (${EXTENSIVE_WARNINGS})
    set_project_warnings(dulcificum)
endif ()

# --- Setup Python bindings ---


# --- Setup tests ---
if (ENABLE_TESTS)
    add_subdirectory(test)
endif ()

# --- Setup Apps ---
if (WITH_APPS)
    add_subdirectory(apps)
endif ()
