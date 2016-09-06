cmake_minimum_required(VERSION 3.6)
project(EECS3540)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES_CopyTree CopyTree/main.cpp)
add_executable(copytree ${SOURCE_FILES_CopyTree})