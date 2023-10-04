set(DULCIFICUM_TEST_DATA_DIR "${CMAKE_SOURCE_DIR}/test/data")
configure_file(test_data_dir.h.in "${CMAKE_SOURCE_DIR}/test/test_data_dir.h")

file(GLOB UNIT_TEST_FILES *.cpp)

add_executable(dulcificum_unit_tester
    ${UNIT_TEST_FILES}
    test_data_dir.h
)

target_include_directories(
    dulcificum_unit_tester PUBLIC
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/miraclegrue_jsontoolpath_dialect"
)

target_link_libraries(dulcificum_unit_tester dulcificum ${CONAN_LIBS})