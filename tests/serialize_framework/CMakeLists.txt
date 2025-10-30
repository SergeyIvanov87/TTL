enable_testing()

add_executable(
  sf_unit_tests
  sf_unit_tests.cpp
)
target_link_libraries(
  sf_unit_tests
  GTest::gtest_main
  Ttl
)

include(GoogleTest)
gtest_discover_tests(sf_unit_tests)
