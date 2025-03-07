file(GLOB_RECURSE SOURCE "*.cpp" "*.hpp")

# HEADER GENERATOR STARTS
set(GENERATOR_EXE ${CMAKE_BINARY_DIR}/deps/header_generator)

if(EXISTS ${GENERATOR_EXE})
  file(REMOVE ${GENERATOR_EXE})
endif()

execute_process(
  COMMAND ${CMAKE_CXX_COMPILER} /std:c++20 ${NGYN_ROOT_DIR}/deps/header_generator/header_generator.cpp -o ${GENERATOR_EXE}
  RESULT_VARIABLE COMPILE_RESULT
  OUTPUT_VARIABLE COMPILE_OUTPUT
  ERROR_VARIABLE COMPILE_ERROR
)

if(NOT COMPILE_RESULT EQUAL 0)
  message(FATAL_ERROR "Failed to compile generator: ${COMPILE_ERROR}")
endif()

execute_process(
  COMMAND ${GENERATOR_EXE} ${NGYN_ROOT_DIR}
  OUTPUT_VARIABLE GENERATED_OUTPUT
  ERROR_VARIABLE RUN_ERROR
)

if(RUN_ERROR)
  message(FATAL_ERROR "Generator execution failed: ${RUN_ERROR}")
endif()
# HEADER GENERATOR ENDS

add_library(ngyn
  ${SOURCE}
  ${NGYN_ROOT_DIR}/deps/glad/src/glad.c
)

target_link_libraries(ngyn PUBLIC
  glfw
  freetype
  Catch2::Catch2WithMain
)

target_include_directories(ngyn PUBLIC
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/include>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/glm>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/stb>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/glad/include>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/rapidjson/include>
)
