cmake_minimum_required(VERSION 3.30)

if (WIN32)
    add_definitions(-DWINDOWS)
elseif (UNIX)
    add_definitions(-DLINUX)
endif()

set(TARGET_NAME common-lib)

project(TARGET_NAME)

set(CMAKE_CXX_STANDARD 17)
set(INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} 
                           PRIVATE
                           ${INCLUDES}
)

# add_subdirectory(test)
