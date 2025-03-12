cmake_minimum_required(VERSION 3.31.1)

set(CMAKE_CXX_STANDARD 20)
set(NGYN_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

project(ngyn CXX)

option(NGYN_BUILD_TESTS "Build ngyn tests" ON)
option(NGYN_BUILD_EXAMPLES "Build ngyn examples" ON)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "Disable HARFBUZZ" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "Disable PNG" FORCE)
set(FT_DISABLE_ZLIB ON CACHE BOOL "Disable ZLIB" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "Disable BZIP2" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "Disable BROTLI" FORCE)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
add_subdirectory(deps/catch2)
add_subdirectory(deps/freetype)

add_subdirectory(src)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(NGYN_BUILD_TESTS)
  message(STATUS "[ngyn] Including tests:")
  enable_testing()
  add_subdirectory(tests)
endif()

if(NGYN_BUILD_EXAMPLES)
  message(STATUS "[ngyn] Including examples:")
  add_subdirectory(examples)
endif()