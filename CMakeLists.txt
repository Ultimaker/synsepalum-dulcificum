cmake_minimum_required(VERSION 3.23)
project(dulcificum)

find_package(standardprojectsettings REQUIRED)
option(EXTENSIVE_WARNINGS "Build with all warnings" ON)
option(ENABLE_TESTS "Build with unit test" ON)
option(WITH_APPS "Build with apps" ON)
option(WITH_JS_BINDINGS "Build with JavaScript Emscripten bindings" OFF)

if(WITH_APPS)
    set(APP_VERSION "0.2.1" CACHE STRING "Version of the translator app" )
    message(STATUS "Configuring translator app version: ${APP_VERSION}")
endif ()

option(WITH_PYTHON_BINDINGS "Build with Python bindings: `pyDulcificum`" ON)
if(WITH_PYTHON_BINDINGS)
    set(PYDULCIFICUM_VERSION "0.2.1" CACHE STRING "Version of the pyDulcificum python bindings" )
    message(STATUS "Configuring pyDulcificum version: ${PYDULCIFICUM_VERSION}")
endif ()

if(NOT DEFINED DULCIFICUM_VERSION)
    message(FATAL_ERROR "DULCIFICUM_VERSION is not defined!")
endif()
message(STATUS "Configuring Dulcificum version: ${DULCIFICUM_VERSION}")
message(STATUS "Configuring Dulcificum hash: ${GIT_COMMIT_HASH}")

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(range-v3 REQUIRED)
find_package(ctre REQUIRED)

# --- Setup the shared C++ mgjtp library ---
set(DULCIFICUM_SRC
        src/gcode/ast/ast.cpp
        src/gcode/ast/entries.cpp
        src/gcode/parse.cpp
        src/miracle_jtp/mgjtp_command_to_json.cpp
        src/miracle_jtp/mgjtp_json_to_command.cpp
        src/miracle_jtp/mgjtp_mappings_json_key_to_str.cpp
        src/gcode/gcode_to_command.cpp
        src/utils/io.cpp
        src/command_types.cpp
)
add_library(dulcificum STATIC ${DULCIFICUM_SRC})

target_link_libraries(dulcificum
        PUBLIC
        nlohmann_json::nlohmann_json
        ctre::ctre
        range-v3::range-v3
        spdlog::spdlog)

target_include_directories(dulcificum
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(dulcificum
    PUBLIC
    DULCIFICUM_VERSION="${DULCIFICUM_VERSION}"
    DULCIFICUM_HASH="${GIT_COMMIT_HASH}"
)

if (NOT WITH_JS_BINDINGS)
    use_threads(dulcificum)
endif ()

enable_sanitizers(dulcificum)
if (${EXTENSIVE_WARNINGS})
    set_project_warnings(dulcificum)
endif ()

# --- Setup JavaScript bindings ---
if (WITH_JS_BINDINGS)
    add_subdirectory(DulcificumJS/cpp)
endif ()

# --- Setup Python bindings ---
if (WITH_PYTHON_BINDINGS)
    add_subdirectory(pyDulcificum)
endif ()

# --- Setup tests ---
if (ENABLE_TESTS)
    add_subdirectory(tests)
endif ()

# --- Setup Apps ---
if (WITH_APPS)
    add_subdirectory(apps)
endif ()
