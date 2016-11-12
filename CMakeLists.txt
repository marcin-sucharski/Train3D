cmake_minimum_required(VERSION 3.6)

project(Train)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})

find_package(SOIL REQUIRED)
include_directories(${SOIL_INCLUDE_DIRS})

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
else()
    find_package(GLFW REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})
endif()

set(LIBRARIES
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${SOIL_LIBRARIES}
    )

add_subdirectory(sources)

set(SOURCE_FILES ${SOURCE_FILES})

add_executable(Train ${SOURCE_FILES})
target_link_libraries(Train ${LIBRARIES})