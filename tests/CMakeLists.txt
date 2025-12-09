cmake_minimum_required (VERSION 3.28)

enable_testing()
project(unit_tests)
include(CTest)

# TODO check this https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake

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
option(SKIP_EVENT_FRAMEWORK_APP_TESTS "Exclude the EventFramework unit test target from a configuration step" OFF)
option(SKIP_RESOURCE_FRAMEWORK_APP_TESTS "Exclude the ResourceFramework unit test target from a configuration step" OFF)
option(SKIP_SERIALIZE_FRAMEWORK_APP_TESTS "Exclude the SerializeFramework unit test target from a configuration step" OFF)

set (REGULAR_TESTS "")
# generate the script, which will generate a coverage report gathered from running of a single regular test suite
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/suite_coverage.sh
"$2/$1\n \
if which lcov;\n \
then \n \
    lcov --ignore-errors inconsistent,inconsistent,empty,mismatch -q --capture --directory $2 --output-file=$1_coverage.info\n \
    lcov --ignore-errors empty -q --extract $1_coverage.info \"${CMAKE_SOURCE_DIR}/*\" --output-file=$1_extracted_coverage.info\n \
    rm $1_coverage.info\n \
    genhtml --ignore-errors mismatch,empty $1_extracted_coverage.info --demangle-cpp --output-directory=$1_coverage_html\n \
fi \
")

# generate the script, which will aggregate coverage reports collected by every single regular unit test,
# so that the aggregated report will reflect the complete test coverage of the project
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/coverage_aggregator.sh
"BASE_DIR=$1\n \
if which lcov;\n \
then\n  \
    FILES_TO_MERGE=\n   \
    for f in `find  $BASE_DIR -name \"*_coverage.info\"`;\n    \
    do\n    \
        FILES_TO_MERGE=\"-a $f $\{FILE_TO_MERGE\}\"\n \
    done\n  \
    lcov $\{FILES_TO_MERGE\} -o all_coverage.info\n    \
    lcov -q --extract all_coverage.info \"${CMAKE_SOURCE_DIR}/*\" --output-file=aggregated_coverage.info\n \
    rm all_coverage.info\n  \
    genhtml --ignore-errors mismatch aggregated_coverage.info --demangle-cpp --output-directory=coverage_html\n  \
fi\n  \
")

# enumerate all unit tests
add_subdirectory(event_framework)
add_subdirectory(applications)

# Introduces an empty test which the only task is to aggregate coverage reports
# of all regular unit tests in a single place by merging measured coverages provided by
# every test suite run
add_test(NAME coverageAggregator
         COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/coverage_aggregator.sh ${CMAKE_CURRENT_BINARY_DIR}
)
# the empty test must be run only when all regular unit tests have passed
set_tests_properties(coverageAggregator PROPERTIES DEPENDS "${REGULAR_TESTS}")
