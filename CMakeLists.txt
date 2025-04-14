cmake_minimum_required(VERSION 3.16)
project(development CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(INCLUDES ${CMAKE_CURRENT_LIST_DIR}/common-lib/include)
set(DEPENDENCIES common-lib)
set(LINKS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

if (WIN32)
    add_definitions(-DWINDOWS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    #set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

    if(DEFINED PYTHON_BASE_DIR)
        message("PYTHON_BASE_DIR = ${PYTHON_BASE_DIR}")
        set(PYTHON_BUILD ON)
        set(PYBIND_BASE_DIR ${PYTHON_BASE_DIR}/Lib/site-packages/pybind11)
        set(CMAKE_PREFIX_PATH "${PYBIND_BASE_DIR}/share/cmake/pybind11" 
                            ${CMAKE_PREFIX_PATH})
        
        find_package(pybind11 REQUIRED)
        find_package(Python3 COMPONENTS Interpreter Development)
    endif()
elseif (UNIX)
    add_definitions(-DLINUX)
endif()

if(DEFINED GTEST_BASE_DIR)
    message("GTEST_BASE_DIR = ${GTEST_BASE_DIR}")
    set(GTEST_ENABLED ON)
endif()

add_subdirectory(common-lib)
# add_subdirectory(engine-lib)
add_subdirectory(application)