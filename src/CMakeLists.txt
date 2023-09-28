include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_library(dulcificum
    ${PROJECT_SOURCE_DIR}/include/command_types.h
    blank.cpp
    blank.h
    makerbot_dialect.cpp
    makerbot_dialect.h
)

target_include_directories(dulcificum PUBLIC ${PROJECT_BINARY_DIR}/include)

target_link_libraries(dulcificum ${CONAN_LIBS})