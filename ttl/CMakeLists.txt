cmake_minimum_required (VERSION 3.28)

# Header-only library
add_library(Ttl INTERFACE)

message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}, INSTALL_INCLUDE_DIR: ${INSTALL_INCLUDE_DIR}" )

target_include_directories(
  Ttl
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..;${CMAKE_CURRENT_SOURCE_DIR}/;${CMAKE_CURRENT_SOURCE_DIR}/include>"
         "$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR};include>"
         )

target_compile_definitions(
  Ttl
  INTERFACE ${COMPILE_DEFS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(Ttl INTERFACE --coverage -O0 -g)
    target_link_libraries(Ttl INTERFACE --coverage)
endif()

add_subdirectory(applications)
set (TTL_APPS_TARGET ${TTL_APPS_TARGET} PARENT_SCOPE)
