add_executable(translator
    translator_main.cpp
)
target_link_libraries(translator dulcificum)
target_include_directories(translator PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(translator PUBLIC "${PROJECT_SOURCE_DIR}/src")
