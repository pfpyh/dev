set(PROJECT_NAME nv_pattern_test)
set(CURRENT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

project(${PROJECT_NAME})

set( PROJECT_INCLUDE_PATH
	 ${CURRENT_PATH}
	 ${ROOT_PATH}/pattern )

file( GLOB_RECURSE PROJECT_SOURCE_FILES
	  ${CURRENT_PATH}/*.h
	  ${CURRENT_PATH}/*.hpp
	  ${CURRENT_PATH}/*.cpp )

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

target_include_directories( ${PROJECT_NAME}
							PRIVATE
							${PROJECT_INCLUDE_PATH} )

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)