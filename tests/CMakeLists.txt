link_libraries(ngyn)
include_directories(${NGYN_ROOT_DIR}/deps/doctest)

# Core executables
add_executable(input_tests core/input_tests.cpp)
add_executable(window_tests core/window_tests.cpp)

# Elements executables
add_executable(sprite_tests elements/sprite_tests.cpp)

# Renderers executables
add_executable(quad_renderer_tests renderers/quad_renderer_tests.cpp)

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