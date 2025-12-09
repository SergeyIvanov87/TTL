cmake_minimum_required (VERSION 3.28)
enable_testing()

set (TARGET_NAME ef_generic_unit_tests)
include(GoogleTest)

set(LIBS "")
if (TARGET Ttl)
    target_sources(Ttl PUBLIC FILE_SET  testSources
                                        TYPE HEADERS
                                        FILES
                                            CMakeLists.txt
                                            CustomCommands.h
                                            CustomConfigurator.h
                                            CustomEvent.h
                                            CustomEventSubscriber.h
                                            fixture.h
                                            ef_unit_tests.cpp
    )
    list(APPEND LIBS
                    Ttl
    )

else()
    find_package(Ttl REQUIRED)
    list(APPEND LIBS
                    Ttl::Ttl
    )
endif()

add_executable(
  ${TARGET_NAME}
  ef_unit_tests.cpp
)

target_link_libraries(
  ${TARGET_NAME}
  INTERFACE --coverage
  PRIVATE
    GTest::gtest_main
    ${LIBS}
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${TARGET_NAME} INTERFACE --coverage -O0 -g)
    target_link_libraries(${TARGET_NAME} INTERFACE --coverage)
endif()

gtest_discover_tests(${TARGET_NAME})
add_test(
    NAME ${TARGET_NAME}
    COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/../suite_coverage.sh ${TARGET_NAME} ${CMAKE_CURRENT_BINARY_DIR}
)

list (APPEND REGULAR_TESTS ${TARGET_NAME})
set (REGULAR_TESTS ${REGULAR_TESTS} PARENT_SCOPE)
