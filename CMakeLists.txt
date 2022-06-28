cmake_minimum_required(VERSION 3.11)

project(dev 
		DESCRIPTION "dev"
		LANGUAGES CXX)
		
set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_PATH}/bld)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_PATH}/bld)

add_subdirectory(pattern)
add_subdirectory(example)