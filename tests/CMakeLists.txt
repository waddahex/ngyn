link_libraries(ngyn)

set(TESTS_SOURCES
  logger_tests
  strings_tests
  files_tests
  window_tests
)

foreach(SOURCE ${TESTS_SOURCES})
  add_executable(${SOURCE} "${SOURCE}.cpp")
  list(APPEND ALL_SOURCES "${SOURCE}.cpp")
endforeach()

add_executable(all_tests ${ALL_SOURCES})