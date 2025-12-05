cmake_minimum_required (VERSION 3.28)
enable_testing()

set (TARGET_NAME rf_unit_tests)

include(GoogleTest)

add_executable(
  ${TARGET_NAME}
  rf_unit_tests.cpp
)
target_compile_options(
  ${TARGET_NAME}
  PRIVATE
  -fconcepts-diagnostics-depth=10
)

if (TARGET Ttl)
    target_sources(Ttl PUBLIC FILE_SET  testSources
                                        TYPE HEADERS
                                        FILES
                                            CMakeLists.txt
                                            CustomResourceLoaders.h
                                            fixture.hpp
                                            ResourceFrameworkSpecs.h
                                            rf_unit_tests.cpp)
    target_link_libraries(
        ${TARGET_NAME}
        PRIVATE
            GTest::gtest_main
            Ttl
    )
else()
    find_package(TtlAppsSerializeFramework REQUIRED)
    target_link_libraries(
        ${TARGET_NAME}
        PRIVATE
            GTest::gtest_main
            Ttl::TtlAppsSerializeFramework
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${TARGET_NAME} INTERFACE --coverage -O0 -g)
endif()

gtest_discover_tests(${TARGET_NAME})

add_test(
    NAME ${TARGET_NAME}
    COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/../suite_coverage.sh ${TARGET_NAME} ${CMAKE_CURRENT_BINARY_DIR}
)

list (APPEND REGULAR_TESTS ${TARGET_NAME})
set (REGULAR_TESTS ${REGULAR_TESTS} PARENT_SCOPE)
