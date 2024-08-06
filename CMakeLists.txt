cmake_minimum_required(VERSION 3.23)
project(dulcificum)

find_package(standardprojectsettings REQUIRED)
option(EXTENSIVE_WARNINGS "Build with all warnings" ON)
option(ENABLE_TESTS "Build with unit test" ON)
option(WITH_APPS "Build with apps" ON)
option(WITH_JS_BINDINGS "Build with JavaScript Emscripten bindings" OFF)

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
find_package(ctre REQUIRED)

# --- Setup the shared C++ mgjtp library ---
set(DULCIFICUM_SRC
        src/gcode/ast/ast.cpp
        src/gcode/ast/entries.cpp
        src/gcode/parse.cpp

        src/miracle_jtp/mgjtp_command_to_json.cpp
        src/miracle_jtp/mgjtp_json_to_command.cpp

        src/gcode/gcode_to_command.cpp

        src/utils/io.cpp
        src/command_types.cpp
)
if (WITH_JS_BINDINGS)
    list(APPEND DULCIFICUM_SRC DulcificumJS/cpp/DulcificumJS.cpp)
    add_executable(dulcificum ${DULCIFICUM_SRC})
    target_link_options(dulcificum
            PUBLIC
            "SHELL:-s EXPORTED_FUNCTIONS=[_gcode_2_miracle_jtp]"
            "SHELL:-s USE_ES6_IMPORT_META=1"
            "SHELL:-s FORCE_FILESYSTEM=1"
            "SHELL:-s EXPORT_NAME=dulcificum"
            "SHELL:-s MODULARIZE=1"
            "SHELL:-s EXPORT_ES6=1"
            "SHELL:-s SINGLE_FILE=1"
            "SHELL:-s ALLOW_MEMORY_GROWTH=1"
            "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
            "SHELL:--bind"
            "SHELL:-l embind"
            "SHELL: --embind-emit-tsd dulcificum.d.ts"
    )
else ()
    add_library(dulcificum ${DULCIFICUM_SRC})
endif ()

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

target_compile_definitions(dulcificum PRIVATE DULCIFICUM_VERSION="${DULCIFICUM_VERSION}")

if (NOT WITH_JS_BINDINGS)
    use_threads(dulcificum)
endif ()

enable_sanitizers(dulcificum)
if (${EXTENSIVE_WARNINGS})
    set_project_warnings(dulcificum)
endif ()

# --- Setup Python bindings ---
if (WITH_PYTHON_BINDINGS)
    add_subdirectory(pyDulcificum)
endif ()

# --- Setup tests ---
if (ENABLE_TESTS)
    add_subdirectory(test)
endif ()

# --- Setup Apps ---
if (WITH_APPS)
    add_subdirectory(apps)
endif ()
