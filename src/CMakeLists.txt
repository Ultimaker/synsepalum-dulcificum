
add_library(dulcificum
    ${PROJECT_SOURCE_DIR}/include/command_types.h
    blank.cpp
    blank.h
)

target_include_directories(dulcificum PUBLIC ${PROJECT_BINARY_DIR}/include)

target_link_libraries(dulcificum ${CONAN_LIBS})