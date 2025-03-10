link_libraries(ngyn Catch2::Catch2WithMain)

set(RESOURCES_TESTS
  font_tests
  resources_manager_tests
  shader_tests
  texture_tests
)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/tests/resources/${CMAKE_BUILD_TYPE}/data)
set(COMMAND_NAME resources_copy_data)

add_custom_target(
  ${COMMAND_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

foreach(SOURCE ${RESOURCES_TESTS})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  add_dependencies(${SOURCE} ${COMMAND_NAME})
  list(APPEND RESOURCES_SOURCES "${SOURCE}.cpp")
endforeach()

add_executable(resources_tests ${RESOURCES_SOURCES})
add_dependencies(resources_tests ${COMMAND_NAME})