cmake_minimum_required(VERSION 3.6)

set(SOURCE_FILES
    ${SOURCE_FILES}

    ${CMAKE_CURRENT_SOURCE_DIR}/image-data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image-loader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/image-loader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh-data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex-definition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex-definition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh-generator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh-generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader-preprocessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shader-preprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mesh-data.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex-format.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vertex-format.cpp

    PARENT_SCOPE)