set(TARGET_NAME TestImu)

set(INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
             ${CMAKE_CURRENT_LIST_DIR}/../common-lib/include)
set(DEPENDENCIES common-lib)
set(LINKS ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

target_include_directories(${TARGET_NAME} 
                           PRIVATE ${INCLUDES})

target_link_directories(${TARGET_NAME}
                        PRIVATE ${LINKS})

target_link_libraries(${TARGET_NAME}
                      PRIVATE ${DEPENDENCIES})
