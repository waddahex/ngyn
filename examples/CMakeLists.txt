link_libraries(ngyn)

set(EXAMPLES_SOURCES
  main
  texture_example
)

set(DATA_DESTINATION ${CMAKE_BINARY_DIR}/examples/${CMAKE_BUILD_TYPE}/data)

add_custom_target(
  copy_data_examples ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${NGYN_ROOT_DIR}/data" ${DATA_DESTINATION}
)

foreach(SOURCE ${EXAMPLES_SOURCES})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  add_dependencies(${SOURCE} copy_data_examples)
  list(APPEND ALL_SOURCES "${SOURCE}.cpp")
endforeach()