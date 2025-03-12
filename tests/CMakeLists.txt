link_libraries(ngyn)
include_directories(${NGYN_ROOT_DIR}/deps/doctest)

add_executable(window_tests core/window_tests.cpp)

add_test(NAME Core_Window COMMAND window_tests)