    find_package(pybind11 REQUIRED)
    find_package(cpython REQUIRED)

    pybind11_add_module(pyDulcificum pyDulcificum.cpp)
    target_link_libraries(pyDulcificum PUBLIC dulcificum pybind11::pybind11 cpython::cpython)
    target_compile_definitions(pyDulcificum PRIVATE PYDULCIFICUM_VERSION="${PYDULCIFICUM_VERSION}")
    if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
        pybind11_strip(pyDulcificum)
    endif()