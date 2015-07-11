cmake_minimum_required(VERSION 3.2)
project(pong)

file(GLOB SOURCE_FILES src/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_executable(pong ${SOURCE_FILES})