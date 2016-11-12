cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    ${SOURCE_FILES}

    ${CMAKE_CURRENT_SOURCE_DIR}/mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shader-program.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shader-program.cpp

    PARENT_SCOPE)