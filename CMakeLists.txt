cmake_minimum_required(VERSION 3.2)
project(pong)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

find_package(GLEW REQUIRED)

find_library(ASSIMP_LIB assimp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
add_definitions(-DGLM_FORCE_RADIANS)

add_executable(pong ${SOURCE_FILES})

#static assets
file(INSTALL textures DESTINATION ./)
file(INSTALL shaders DESTINATION ./)

target_link_libraries(pong ${GLEW_LIBRARIES})
target_link_libraries(pong ${GLFW_STATIC_LIBRARIES})
target_link_libraries(pong ${ASSIMP_LIB})

