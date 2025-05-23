set(TARGET_NAME test-${TARGET_NAME})
project(${TARGET_NAME})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

set(INCLUDES ${INCLUDES}
             ${TEST_INCLUDES}
             ${CMAKE_CURRENT_LIST_DIR}/include)

set(LINKS ${LINKS}
          ${TEST_LINKS})

set(DEPENDENCIES ${DEPENDENCIES}
                 ${TEST_DEPENDENCIES}
                 common-lib
                 engine-lib)

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
