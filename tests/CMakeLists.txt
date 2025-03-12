link_libraries(ngyn)
include_directories(${NGYN_ROOT_DIR}/deps/doctest)

add_executable(input_tests core/input_tests.cpp)
add_executable(window_tests core/window_tests.cpp)

add_test(NAME Core_Input COMMAND input_tests)
add_test(NAME Core_Window COMMAND window_tests)