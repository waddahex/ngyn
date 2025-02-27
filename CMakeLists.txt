cmake_minimum_required(VERSION 3.31.1)

set(CMAKE_CXX_STANDARD 20)
set(NGYN_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

project(ngyn CXX)

option(NGYN_BUILD_TESTS "Build ngyn tests" ON)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

add_subdirectory(src)
add_subdirectory(deps/glfw)

if(NGYN_BUILD_TESTS)
  add_subdirectory(tests)
endif()