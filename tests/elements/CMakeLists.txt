link_libraries(ngyn Catch2::Catch2WithMain)

set(ELEMENTS_TESTS
  sprite_tests
  text_tests
)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/tests/elements/${CMAKE_BUILD_TYPE}/data)
set(COMMAND_NAME elements_copy_data)

add_custom_target(
  ${COMMAND_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

foreach(SOURCE ${ELEMENTS_TESTS})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  add_dependencies(${SOURCE} ${COMMAND_NAME})
  list(APPEND ELEMENTS_SOURCES "${SOURCE}.cpp")
endforeach()

add_executable(elements_tests ${ELEMENTS_SOURCES})
add_dependencies(elements_tests ${COMMAND_NAME})