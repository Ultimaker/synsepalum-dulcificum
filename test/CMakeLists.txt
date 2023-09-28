add_executable(dulcificum_unit_tester
    dulcificum_unit_tester.cpp
)

target_link_libraries(dulcificum_unit_tester dulcificum ${CONAN_LIBS})