link_libraries(ngyn Catch2::Catch2WithMain)

set(COMPONENTS_TESTS
  frame_tests
)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/tests/components/${CMAKE_BUILD_TYPE}/data)
set(COMMAND_NAME components_copy_data)

add_custom_target(
  ${COMMAND_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

foreach(SOURCE ${COMPONENTS_TESTS})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  add_dependencies(${SOURCE} ${COMMAND_NAME})
  list(APPEND COMPONENTS_SOURCES "${SOURCE}.cpp")
endforeach()

add_executable(components_tests ${COMPONENTS_SOURCES})
add_dependencies(components_tests ${COMMAND_NAME})