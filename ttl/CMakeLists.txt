cmake_minimum_required (VERSION 3.23)

# Header-only library
add_library(Ttl INTERFACE)

target_include_directories(
  Ttl
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..;${CMAKE_CURRENT_SOURCE_DIR}/;${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_SOURCE_DIR}/applications>"
         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR};include>"
         )

target_compile_definitions(
  Ttl
  INTERFACE ${COMPILE_DEFS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(Ttl INTERFACE --coverage -O0 -g)
    target_link_libraries(Ttl INTERFACE --coverage)
endif()

#set_target_properties(Ttl PROPERTIES
#  PUBLIC_HEADER include/Ttl.hpp)

install(TARGETS Ttl
  EXPORT TtlTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  FILE_SET testSources DESTINATION "${INSTALL_EXAMPLE_DIR}" COMPONENT examples
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
    COMPONENT dev)

install(DIRECTORY ./ ./include ./applications DESTINATION "${INSTALL_INCLUDE_DIR}"
        FILES_MATCHING PATTERN "*.h*")
