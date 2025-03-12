link_libraries(ngyn)
include_directories(${NGYN_ROOT_DIR}/deps/doctest)

add_executable(input_tests core/input_tests.cpp)
add_executable(window_tests core/window_tests.cpp)
add_executable(sprite_tests elements/sprite_tests.cpp)
add_executable(quad_renderer_tests renderers/quad_renderer_tests.cpp)

add_test(NAME Core_Input COMMAND input_tests)
add_test(NAME Core_Window COMMAND window_tests)
add_test(NAME Elements_Sprite COMMAND sprite_tests)
add_test(NAME Renderers_Quad_Renderer COMMAND quad_renderer_tests)