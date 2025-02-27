cmake_minimum_required(VERSION 3.31.1)

set(CMAKE_CXX_STANDARD 20)
set(DDENGINE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

project(ddengine CXX)

option(DDENGINE_BUILD_TESTS "Build ddengine tests" ON)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

add_subdirectory(src)
add_subdirectory(deps/glfw)

if(DDENGINE_BUILD_TESTS)
  add_subdirectory(tests)
endif()