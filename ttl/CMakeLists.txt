cmake_minimum_required (VERSION 3.15)

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


#set_target_properties(Ttl PROPERTIES
#  PUBLIC_HEADER include/Ttl.hpp)

install(TARGETS Ttl
  EXPORT TtlTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
    COMPONENT dev)

install(DIRECTORY ./ ./include ./applications DESTINATION "${INSTALL_INCLUDE_DIR}"
        FILES_MATCHING PATTERN "*.h*")

install(DIRECTORY ../cmake DESTINATION "${INSTALL_LIB_DIR}"
        FILES_MATCHING PATTERN "*")
