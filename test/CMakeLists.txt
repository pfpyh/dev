set(TARGET_NAME test-${TARGET_NAME})
project(${TARGET_NAME})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

set(INCLUDES ${INCLUDES}
             ${GTEST_BASE_DIR}/googletest/include)

if (WIN32)
    set(LINKS ${LINKS}
              ${GTEST_BASE_DIR}/build/lib/Debug)
elseif (UNIX)
    set(LINKS ${LINKS}
              ${GTEST_BASE_DIR}/build/lib/)
endif()

set(DEPENDENCIES ${DEPENDENCIES}
                 common-lib
                 gtest)

add_executable(${TARGET_NAME} ${SOURCES})
add_test(NAME ${TARGET_NAME}
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME})

target_include_directories(${TARGET_NAME}
                           PRIVATE 
                           ${INCLUDES})

target_link_directories(${TARGET_NAME}
                        PRIVATE 
                        ${LINKS})

target_link_libraries(${TARGET_NAME}
                      PRIVATE 
                      ${DEPENDENCIES})
