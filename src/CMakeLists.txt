add_library(dulcificum
    miraclegrue_jsontoolpath_dialect.cpp
    ../include/command_types.cpp
)

target_include_directories(dulcificum INTERFACE ${PROJECT_BINARY_DIR}/include)

target_link_libraries(dulcificum PUBLIC ${CONAN_LIBS})
