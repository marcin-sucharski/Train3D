cmake_minimum_required(VERSION 3.6)

add_subdirectory(di)
add_subdirectory(util)
add_subdirectory(env)
add_subdirectory(gfx)
add_subdirectory(res)
add_subdirectory(input)
add_subdirectory(model)

set(SOURCE_FILES
    ${SOURCE_FILES}

    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ogl.h

    PARENT_SCOPE
    )