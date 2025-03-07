link_libraries(ngyn Catch2::Catch2WithMain)

set(CORE_TESTS
  input_tests
  window_tests
)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/tests/core/${CMAKE_BUILD_TYPE}/data)
set(COMMAND_NAME core_copy_data)

add_custom_target(
  ${COMMAND_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

foreach(SOURCE ${CORE_TESTS})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  add_dependencies(${SOURCE} ${COMMAND_NAME})
  list(APPEND CORE_SOURCES "${SOURCE}.cpp")
endforeach()

add_executable(core_tests ${CORE_SOURCES})
add_dependencies(core_tests ${COMMAND_NAME})