link_libraries(ngyn Catch2::Catch2WithMain)

set(RENDERERS_TESTS
  quad_renderer_tests
)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/tests/renderers/${CMAKE_BUILD_TYPE}/data)
set(COMMAND_NAME renderers_copy_data)

add_custom_target(
  ${COMMAND_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

foreach(SOURCE ${RENDERERS_TESTS})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  add_dependencies(${SOURCE} ${COMMAND_NAME})
  list(APPEND RENDERERS_SOURCES "${SOURCE}.cpp")
endforeach()

add_executable(renderers_tests ${RENDERERS_SOURCES})
add_dependencies(renderers_tests ${COMMAND_NAME})