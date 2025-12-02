cmake_minimum_required (VERSION 3.28)
enable_testing()

set (TARGET_NAME ef_unit_tests)
add_executable(
  ${TARGET_NAME}
  ef_unit_tests.cpp
)

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})

target_link_libraries(
  ${TARGET_NAME}
  INTERFACE --coverage
  PRIVATE
  GTest::gtest_main
)

if (TARGET Ttl)
    target_sources(Ttl PUBLIC FILE_SET  testSources
                                        TYPE HEADERS
                                        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
                                        FILES
                                            CMakeLists.txt
                                            CustomCommands.h
                                            CustomConfigurator.h
                                            CustomEvent.h
                                            CustomEventSubscriber.h
                                            fixture.h
                                            ef_unit_tests.cpp)
    target_link_libraries(
        ${TARGET_NAME}
        PRIVATE
            GTest::gtest_main
            Ttl
    )

else()
    find_package(TtlAppsEventFramework REQUIRED)
    target_link_libraries(
        ${TARGET_NAME}
        PRIVATE
            GTest::gtest_main
            Ttl::TtlAppsEventFramework
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${TARGET_NAME} INTERFACE --coverage -O0 -g)
endif()

add_test(
    NAME ${TARGET_NAME}
    COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/../suite_coverage.sh ${TARGET_NAME} ${CMAKE_CURRENT_BINARY_DIR}
)

list (APPEND REGULAR_TESTS ${TARGET_NAME})
set (REGULAR_TESTS ${REGULAR_TESTS} PARENT_SCOPE)
