link_libraries(ngyn Catch2::Catch2WithMain)

set(UTIL_TESTS
  color_tests
  files_tests
  logger_tests
  random_tests
  strings_tests
)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/tests/util/${CMAKE_BUILD_TYPE}/data)
set(COMMAND_NAME util_copy_data)

add_custom_target(
  ${COMMAND_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

foreach(SOURCE ${UTIL_TESTS})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  add_dependencies(${SOURCE} ${COMMAND_NAME})
  list(APPEND UTIL_SOURCES "${SOURCE}.cpp")
endforeach()

add_executable(util_tests ${UTIL_SOURCES})
add_dependencies(util_tests ${COMMAND_NAME})