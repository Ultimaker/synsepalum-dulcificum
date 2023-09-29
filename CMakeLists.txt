cmake_minimum_required(VERSION 3.16)
project(dulcificum)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#------------------------------#
#    SETUP CONAN
#------------------------------#
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
        "${CMAKE_BINARY_DIR}/conan.cmake")
endif ()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.py
    BASIC_SETUP TARGETS
    BUILD missing
    #        KEEP_RPATHS
)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

#------------------------------#
#    SUBDIRECTORIES
#------------------------------#
add_subdirectory(apps)
add_subdirectory(src)
add_subdirectory(test)