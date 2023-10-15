find_package(docopt REQUIRED)


add_executable(translator
        translator_main.cpp
)

enable_sanitizers(translator)
if (${EXTENSIVE_WARNINGS})
    set_project_warnings(translator)
endif ()

target_link_libraries(translator PUBLIC dulcificum docopt_s spdlog::spdlog)
target_compile_definitions(translator PRIVATE APP_VERSION="${APP_VERSION}")
