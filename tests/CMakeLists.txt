cmake_minimum_required (VERSION 3.23)

enable_testing()
project(unit_tests)
include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (TARGET Ttl)
    target_sources(Ttl PUBLIC FILE_SET  testSources
                                        TYPE HEADERS
                                        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                                        FILES
                                            CMakeLists.txt)
endif()


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Exclude gtest from installation
option(INSTALL_GTEST "Enable installation of googletest." OFF)
FetchContent_MakeAvailable(googletest)

# specify a list of regular unit tests to execute
set (REGULAR_TESTS "")
# generate the script, which will generate a coverage report gathered from running of a single regular test suite
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/suite_coverage.sh
"$2/$1 && lcov --ignore-errors inconsistent,inconsistent -q --capture --directory $2 --output-file=$1_coverage.info && lcov -q --extract $1_coverage.info \"${CMAKE_SOURCE_DIR}/*\" --output-file=$1_extracted_coverage.info && rm $1_coverage.info  && genhtml $1_extracted_coverage.info --demangle-cpp --output-directory=$1_coverage_html")

# generate the script, which will aggregate coverage reports collected by every single regular unit test,
# so that the aggregated report will reflect the complete test coverage of the project
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/coverage_aggregator.sh
"lcov -a \"*/*_coverage.info\" -o all_coverage.info && lcov -q --extract all_coverage.info \"${CMAKE_SOURCE_DIR}/*\" --output-file=aggregated_coverage.info && rm all_coverage.info  && genhtml --ignore-errors range aggregated_coverage.info --demangle-cpp --output-directory=coverage_html")

# enumerate all regular unit tests
add_subdirectory(event_framework)
add_subdirectory(resource_framework)
add_subdirectory(serialize_framework)

# Introduces an empty test which the only task is to aggregate coverage reports
# of all regular unit tests in a single place by merging measured coverages provided by
# every test suite run
add_test(NAME coverageAggregator
         COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/coverage_aggregator.sh
)
# the empty test must be run only when all regular unit tests have passed
set_tests_properties(coverageAggregator PROPERTIES DEPENDS "${REGULAR_TESTS}")
