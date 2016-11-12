cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    ${SOURCE_FILES}

    ${CMAKE_CURRENT_SOURCE_DIR}/camera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/controllable-camera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/controllable-camera.cpp

    PARENT_SCOPE)