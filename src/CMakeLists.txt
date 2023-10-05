add_subdirectory(miraclegrue_jsontoolpath_dialect)
list(TRANSFORM MIRACLEGRUE_TOOLPATH_DIALECT_SRC PREPEND miraclegrue_jsontoolpath_dialect/)

add_library(dulcificum
    command_types.cpp
    ../include/command_types.h
    ${MIRACLEGRUE_TOOLPATH_DIALECT_SRC}
)

target_include_directories(dulcificum PUBLIC miraclegrue_jsontoolpath_dialect)
target_include_directories(dulcificum INTERFACE ${PROJECT_BINARY_DIR}/include)

target_link_libraries(dulcificum PUBLIC ${CONAN_LIBS})
