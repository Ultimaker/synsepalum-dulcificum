set(ENV{LD_LIBRARY_PATH} "${CMAKE_LIBRARY_PATH}:${LD_LIBRARY_PATH}")  # Needed to ensure that CMake finds the Conan CPython library
find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

pybind11_add_module(pyDulcificum pyDulcificum.cpp)
target_link_libraries(pyDulcificum PUBLIC dulcificum pybind11::pybind11)
target_compile_definitions(pyDulcificum PRIVATE PYDULCIFICUM_VERSION="${PYDULCIFICUM_VERSION}")
if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
    pybind11_strip(pyDulcificum)
endif()
