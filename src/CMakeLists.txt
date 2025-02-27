file(GLOB_RECURSE SOURCE "*.cpp" "*.hpp")

add_library(ddengine
  ${SOURCE}
  ${DDENGINE_ROOT_DIR}/deps/glad/src/glad.c
)

target_link_libraries(ddengine PUBLIC
  glfw
)

target_include_directories(ddengine PUBLIC
  $<BUILD_INTERFACE:${DDENGINE_ROOT_DIR}/src>
  $<BUILD_INTERFACE:${DDENGINE_ROOT_DIR}/deps/glm>
  $<BUILD_INTERFACE:${DDENGINE_ROOT_DIR}/deps/glad/include>
)