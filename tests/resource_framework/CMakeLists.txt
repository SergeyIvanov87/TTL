cmake_minimum_required (VERSION 3.23)

enable_testing()

set (TARGET_NAME rf_unit_tests)

if (TARGET Ttl)
    target_sources(Ttl PUBLIC FILE_SET  testSources
                                        TYPE HEADERS
                                        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
                                        FILES
                                            CMakeLists.txt
                                            CustomResourceLoaders.h
                                            fixture.hpp
                                            ResourceFrameworkSpecs.h
                                            rf_unit_tests.cpp)
else()
    find_package(Ttl REQUIRED)
endif()

add_executable(
  ${TARGET_NAME}
  rf_unit_tests.cpp
)
target_compile_options(
  ${TARGET_NAME}
  PRIVATE
  -fconcepts-diagnostics-depth=10
)

target_link_libraries(
  ${TARGET_NAME}
  GTest::gtest_main
  Ttl
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${TARGET_NAME} INTERFACE --coverage -O0 -g)
endif()

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})

# add_test doesn't allow to enumerate commands in pipeline, create a specific script-wrapper to invoke lcov
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/coverage.sh
"${CMAKE_CURRENT_BINARY_DIR}/$1 && lcov --ignore-errors inconsistent,inconsistent -q --capture --directory . --output-file=all_coverage.info && lcov -q --extract all_coverage.info \"${CMAKE_SOURCE_DIR}/*\" --output-file=coverage.info && rm all_coverage.info  && genhtml coverage.info --demangle-cpp --output-directory=coverage_html")

add_test(
    NAME ${TARGET_NAME}
    COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/coverage.sh ${TARGET_NAME}
)
