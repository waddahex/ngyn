file(GLOB_RECURSE SOURCE "*.cpp" "*.hpp")

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
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/src>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/glm>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/stb>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/glad/include>
  $<BUILD_INTERFACE:${NGYN_ROOT_DIR}/deps/rapidjson/include>
)