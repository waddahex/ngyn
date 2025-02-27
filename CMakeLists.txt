cmake_minimum_required(VERSION 3.31.1)

set(CMAKE_CXX_STANDARD 20)
project(ddengine CXX)

option(DDENGINE_BUILD_TESTS "Build ddengine tests" ON)

# add_subdirectory(src)

if(DDENGINE_BUILD_TESTS)
  add_subdirectory(tests)
endif()