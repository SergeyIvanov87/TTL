cmake_minimum_required (VERSION 3.28)
enable_testing()

set (TARGET_NAME sf_unit_tests)
add_executable(${TARGET_NAME})

set (SOURCES sf_unit_tests.cpp)
target_sources(${TARGET_NAME}
    PRIVATE
        ${SOURCES}
)

set (LIBS GTest::gtest_main)
if (TARGET Ttl)
    target_sources(Ttl PUBLIC FILE_SET  testSources
                                        TYPE HEADERS
                                        FILES
                                            CMakeLists.txt
                                            ${SOURCES})
    #-S- list (APPEND LIBS Ttl)
else()
    find_package(TtlAppsSerializeFramework REQUIRED)
    list (APPEND LIBS Ttl::TtlAppsSerializeFramework)
endif()

target_link_libraries(
  ${TARGET_NAME}
  ${LIBS}
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${TARGET_NAME} INTERFACE --coverage -O0 -g)
endif()

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})

add_test(
    NAME ${TARGET_NAME}
    COMMAND sh ${CMAKE_CURRENT_BINARY_DIR}/../suite_coverage.sh ${TARGET_NAME} ${CMAKE_CURRENT_BINARY_DIR}
)

list (APPEND REGULAR_TESTS ${TARGET_NAME})
set (REGULAR_TESTS ${REGULAR_TESTS} PARENT_SCOPE)
