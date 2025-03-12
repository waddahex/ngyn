link_libraries(ngyn)
include_directories(${NGYN_ROOT_DIR}/deps/doctest)

# Core executables
add_executable(input_tests core/input_tests.cpp)
add_executable(window_tests core/window_tests.cpp)

# Elements executables
add_executable(sprite_tests elements/sprite_tests.cpp)

# Renderers executables
add_executable(quad_renderer_tests renderers/quad_renderer_tests.cpp)

# Resources executables
add_executable(font_tests resources/font_tests.cpp)
add_executable(resources_manager_tests resources/resources_manager_tests.cpp)
add_executable(shader_tests resources/shader_tests.cpp)
add_executable(texture_tests resources/texture_tests.cpp)

# Util executables
add_executable(color_tests util/color_tests.cpp)
add_executable(files_tests util/files_tests.cpp)
add_executable(logger_tests util/logger_tests.cpp)
add_executable(random_tests util/random_tests.cpp)
add_executable(strings_tests util/strings_tests.cpp)

# Core tests
add_test(NAME Core_Input COMMAND input_tests)
add_test(NAME Core_Window COMMAND window_tests)

# Elements tests
add_test(NAME Elements_Sprite COMMAND sprite_tests)

# Renderer tests
add_test(NAME Renderers_Quad_Renderer COMMAND quad_renderer_tests)

# Util tests
add_test(NAME Util_Color COMMAND color_tests)
add_test(NAME Util_Files COMMAND files_tests)
add_test(NAME Util_Logger COMMAND logger_tests)
add_test(NAME Util_Random COMMAND random_tests)
add_test(NAME Util_Strings COMMAND strings_tests)

# Resources tests
add_test(NAME Resources_Font COMMAND font_tests ${NGYN_ROOT_DIR})
add_test(NAME Resources_Resources_Manager COMMAND resources_manager_tests ${NGYN_ROOT_DIR})
add_test(NAME Resources_Shader COMMAND shader_tests ${NGYN_ROOT_DIR})
add_test(NAME Resources_Texture COMMAND texture_tests ${NGYN_ROOT_DIR})

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/tests/${CMAKE_BUILD_TYPE}/data)

add_custom_target(
  copy_data_tests ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

set(DATA_DEPENDENTS
  font_tests
)

foreach(SOURCE ${DATA_DEPENDENTS})
  add_dependencies(${SOURCE} copy_data_tests)
endforeach()