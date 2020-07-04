# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
    FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
    ")
endif()

cmake_minimum_required(VERSION 2.8)
project(MediaPlatform)

set(Tutorial_VERSION_MAJOR 0)
set(Tutorial_VERSION_MINOR 1)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
set(CMAKE_VERBOSE_MAKEFILE on)

message("CMAKE_SOURCE_DIR:${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")



ADD_DEFINITIONS("-std=c++11")

ADD_DEFINITIONS("-g -DDEBUG")

add_subdirectory(utils)
add_subdirectory(src)