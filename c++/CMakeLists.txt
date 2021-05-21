cmake_minimum_required(VERSION 3.0.0)
project(word-search)

include_directories(${CMAKE_SOURCE_DIR}/includes)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")
add_executable(word-search ${SOURCES})