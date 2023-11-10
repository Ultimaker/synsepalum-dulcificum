message(STATUS "dulcificum: Compiling with Tests")
enable_testing()

find_package(GTest REQUIRED)

set(UNIT_TEST_FILES
        ack.cpp
        dulcificum_unit_tester.cpp
        miracle_jsontoolpath_dialect_test.cpp
)

add_executable(dulcificum_unit_tester
    ${UNIT_TEST_FILES}
    test_data_dir.h
)

target_link_libraries(dulcificum_unit_tester PUBLIC dulcificum GTest::gtest GTest::gmock)