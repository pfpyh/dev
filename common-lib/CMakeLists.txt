set(TARGET_NAME common-lib)

set(INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)
set(DEPENDENCIES)
set(LINKS ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_BINARY_DIR}/Debug
          ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

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
