file(GLOB UNIT_TEST_FILES *.cpp)

add_executable(dulcificum_unit_tester
    dulcificum_unit_tester.cpp
    ${UNIT_TEST_FILES}
)

target_link_libraries(dulcificum_unit_tester dulcificum ${CONAN_LIBS})