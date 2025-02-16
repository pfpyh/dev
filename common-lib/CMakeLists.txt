set(TARGET_NAME common-lib)

set(INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
             ${INCLUDES})

set(LINKS ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_BINARY_DIR}/Debug
          ${CMAKE_CURRENT_BINARY_DIR}/Release
          ${LINKS})

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_library(${TARGET_NAME} SHARED ${SOURCES})

target_include_directories(${TARGET_NAME} 
                           PRIVATE ${INCLUDES})

target_link_directories(${TARGET_NAME}
                        PRIVATE ${LINKS})

if(PYTHON_BUILD)
    set(PY_TARGET_NAME py_common_lib)

    set(INCLUDES ${PYTHON_BASE_DIR}/include
                 ${PYBIND_BASE_DIR}/include
                 ${INCLUDES})

    set(LINKS ${PYTHON_BASE_DIR}/libs
              ${LINKS})

    set(DEPENDENCIES ${TARGET_NAME}
                     pybind11::module 
                     Python3::Python)

    file(GLOB_RECURSE PY_SOURCES ${CMAKE_CURRENT_LIST_DIR}/python/*.cpp)

    pybind11_add_module(${PY_TARGET_NAME} SHARED ${PY_SOURCES})

    target_include_directories(${PY_TARGET_NAME} 
                               PRIVATE ${INCLUDES})

    target_link_directories(${PY_TARGET_NAME}
                            PRIVATE ${LINKS})

    target_link_libraries(${PY_TARGET_NAME} 
                          PRIVATE ${DEPENDENCIES})
endif()

if(GTEST_ENABLED)
    add_subdirectory(test)
endif()
