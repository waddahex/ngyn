file(GLOB_RECURSE SOURCE "*.cpp" "*.hpp")

add_library(ngyn
  ${SOURCE}
  ${NGYN_ROOT_DIR}/deps/glad/src/glad.c
)

target_link_libraries(ngyn PUBLIC
  glfw
)

target_include_directories(ngyn PUBLIC
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/src>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/glm>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/glad/include>
)