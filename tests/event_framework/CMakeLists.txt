cmake_minimum_required (VERSION 3.23)

enable_testing()

if (TARGET Ttl)
    target_sources(Ttl PUBLIC FILE_SET  testSources
                                        TYPE HEADERS
                                        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
                                        FILES
                                            CMakeLists.txt
                                            CustomCommands.h
                                            CustomEvent.h
                                            ef_unit_tests.cpp)
else()
    find_package(Ttl REQUIRED)
endif()


add_executable(
  ef_unit_tests
  ef_unit_tests.cpp
)
target_link_libraries(
  ef_unit_tests
  INTERFACE --coverage
  PRIVATE
  GTest::gtest_main
  Ttl
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(ef_unit_tests INTERFACE --coverage -O0 -g)
endif()

include(GoogleTest)
gtest_discover_tests(ef_unit_tests)

# add_test doesn't allow to enumerate commands in pipeline, create a specific script-wrapper to invoke lcov
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/coverage.sh
"${CMAKE_CURRENT_BINARY_DIR}/$1 && lcov --ignore-errors inconsistent,inconsistent -q --capture --directory . --output-file=all_coverage.info && lcov -q --extract all_coverage.info \"${CMAKE_SOURCE_DIR}/*\" --output-file=coverage.info && rm all_coverage.info  && genhtml coverage.info --demangle-cpp --output-directory=coverage_html")

add_test(
    NAME ef_unit_tests
    COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/coverage.sh ef_unit_tests
)
