cmake_minimum_required(VERSION 3.16)
project(development CXX)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    add_definitions(-DWINDOWS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    #set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
elseif (UNIX)
    add_definitions(-DLINUX)
endif()

add_subdirectory(common-lib)
add_subdirectory(application)