cmake_minimum_required (VERSION 3.28)

enable_testing()
project(applications_unit_tests)
include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (TARGET Ttl)
    target_sources(Ttl PUBLIC FILE_SET  testSources
                                        TYPE HEADERS
                                    FILES
                                        CMakeLists.txt)
endif()


# enumerate all regular unit tests
if (NOT SKIP_EVENT_FRAMEWORK_APP_TESTS)
    add_subdirectory(event_framework)
endif()

if (NOT SKIP_RESOURCE_FRAMEWORK_APP_TESTS)
    add_subdirectory(resource_framework)
endif()

if (NOT SKIP_SERIALIZE_FRAMEWORK_APP_TESTS)
    add_subdirectory(serialize_framework)
endif()

set (REGULAR_TESTS ${REGULAR_TESTS} PARENT_SCOPE)
