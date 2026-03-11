add_executable(dulcificum_js DulcificumJS.cpp)

# Emscripten defaults MAXIMUM_MEMORY to 2GB when growth is enabled.
# Expose memory limits as cache variables so downstream builds can tune them.
set(DULCIFICUM_JS_INITIAL_MEMORY "256MB" CACHE STRING "Initial wasm heap for dulcificum_js")
set(DULCIFICUM_JS_MAXIMUM_MEMORY "4GB" CACHE STRING "Maximum wasm heap for dulcificum_js when growth is enabled")

target_link_options(dulcificum_js
        PUBLIC
        "SHELL:-s USE_ES6_IMPORT_META=1"
        "SHELL:-s FORCE_FILESYSTEM=1"
        "SHELL:-s EXPORT_NAME=dulcificum"
        "SHELL:-s MODULARIZE=1"
        "SHELL:-s EXPORT_ES6=1"
        "SHELL:-s SINGLE_FILE=1"
        "SHELL:-s ALLOW_MEMORY_GROWTH=1"
        "SHELL:-s INITIAL_MEMORY=${DULCIFICUM_JS_INITIAL_MEMORY}"
        "SHELL:-s MAXIMUM_MEMORY=${DULCIFICUM_JS_MAXIMUM_MEMORY}"
        "SHELL:-s ERROR_ON_UNDEFINED_SYMBOLS=0"
        "SHELL:--bind"
        "SHELL:-l embind"
        "SHELL: --embind-emit-tsd dulcificum_js.d.ts"
)
target_link_libraries(dulcificum_js PUBLIC dulcificum)
