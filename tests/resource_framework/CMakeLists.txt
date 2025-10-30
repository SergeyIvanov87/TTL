enable_testing()

add_executable(
  rf_unit_tests
  rf_unit_tests.cpp
)
target_link_libraries(
  rf_unit_tests
  GTest::gtest_main
  Ttl
)

include(GoogleTest)
gtest_discover_tests(rf_unit_tests)
