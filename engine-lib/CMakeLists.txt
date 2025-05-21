set(TARGET_NAME engine-lib)

set(INCLUDES ${INCLUDES}
             ${CMAKE_CURRENT_LIST_DIR}/include)

set(LINKS ${LINKS}
          ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_BINARY_DIR}/Debug
          ${CMAKE_CURRENT_BINARY_DIR}/Release)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(${TARGET_NAME} SHARED ${SOURCES})

target_include_directories(${TARGET_NAME} 
                           PRIVATE ${INCLUDES})

target_link_libraries(${TARGET_NAME} 
                      PRIVATE ${DEPENDENCIES})

target_link_directories(${TARGET_NAME}
                        PRIVATE ${LINKS})

add_subdirectory(test)
                    