link_libraries(ngyn)

set(TESTS_SOURCES
  files_tests
  input_tests
  logger_tests
  quad_renderer_tests
  strings_tests
  shader_tests
  texture_tests
  window_tests
)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/tests/${CMAKE_BUILD_TYPE}/data)

add_custom_target(
  copy_data ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

foreach(SOURCE ${TESTS_SOURCES})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  add_dependencies(${SOURCE} copy_data)
  list(APPEND ALL_SOURCES "${SOURCE}.cpp")
endforeach()

add_executable(all_tests ${ALL_SOURCES})
add_dependencies(all_tests copy_data)