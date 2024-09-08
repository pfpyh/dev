cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(TARGET_NAME common-lib)
project(${TARGET_NAME})

set(INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)
set(DEPENDENCIES)
set(LINKS ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

if (WIN32)
    add_definitions(-DWINDOWS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
elseif (UNIX)
    add_definitions(-DLINUX)
endif()

add_library(${TARGET_NAME} STATIC ${SOURCES})

target_include_directories(${TARGET_NAME} 
                           PRIVATE ${INCLUDES})

target_link_directories(${TARGET_NAME}
                        PRIVATE ${LINKS})

target_link_libraries(${TARGET_NAME}
                      PRIVATE ${DEPENDENCIES})

if(DEFINED GTEST_BASE_DIR)
    message("GTEST_BASE_DIR = ${GTEST_BASE_DIR}")
    add_subdirectory(test)
endif()
