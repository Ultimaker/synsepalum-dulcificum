file(GLOB UNIT_TEST_FILES *.cpp)

add_executable(dulcificum_unit_tester
    ${UNIT_TEST_FILES}
)

target_include_directories(dulcificum_unit_tester PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(dulcificum_unit_tester dulcificum ${CONAN_LIBS})