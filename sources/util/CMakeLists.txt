cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    ${SOURCE_FILES}

    ${CMAKE_CURRENT_SOURCE_DIR}/curve-provider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/time-step.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/time-step.h

    PARENT_SCOPE
    )