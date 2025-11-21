cmake_minimum_required (VERSION 3.23)

project(Ttl CXX)

set (BRANCH_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(BRANCH_ROOT ${BRANCH_ROOT} ABSOLUTE)

set (PROCESS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(PROCESS_DIR ${PROCESS_DIR} ABSOLUTE)

# configure version
set(TTL_MAJOR_VERSION 0)
set(TTL_MINOR_VERSION 1)
set(TTL_PATCH_VERSION 0)
set(TTL_VERSION
    ${TTL_MAJOR_VERSION}.${TTL_MINOR_VERSION}.${TTL_PATCH_VERSION})

# set install_path's
set(INSTALL_LIB_DIR lib/ttl CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include/ttl CACHE PATH "Installation directory for header files")
set(INSTALL_EXAMPLE_DIR examples CACHE PATH "Installation directory for examples files")

if(WIN32 AND NOT CYGWIN)
message ("DEFAULT_INSTALL_CMAKE_DIR: ${DEFAULT_INSTALL_CMAKE_DIR}")
  set(DEFAULT_INSTALL_CMAKE_DIR CMake)
else()
  set(DEFAULT_INSTALL_CMAKE_DIR lib/cmake/Ttl)
endif()
message ("DEFAULT_INSTALL_CMAKE_DIR: ${DEFAULT_INSTALL_CMAKE_DIR}")

set(INSTALL_CMAKE_DIR ${DEFAULT_INSTALL_CMAKE_DIR} CACHE PATH   "Installation directory for CMake files")
message ("INSTALL_CMAKE_DIR: ${INSTALL_CMAKE_DIR}")

# make install_path's absolute
foreach(var LIB BIN INCLUDE CMAKE)
  set(path INSTALL_${var}_DIR)
  if(NOT IS_ABSOLUTE "${${path}}")
    set(${path} "${CMAKE_INSTALL_PREFIX}/${${path}}")
  endif()
endforeach()


include_directories(
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_BINARY_DIR}")


add_subdirectory(ttl)
add_subdirectory(examples)
add_subdirectory(tests)

export(TARGETS Ttl ${EXAMPLE_TARGETS_EXPORT}
       FILE "${PROJECT_BINARY_DIR}/Ttl.cmake")
export(PACKAGE Ttl)

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(TtlConfig.cmake.in
  "${PROJECT_BINARY_DIR}/TtlConfig.cmake" @ONLY)

set(CONF_INCLUDE_DIRS "\${PROJECT_SOURCE_DIR}/${REL_INCLUDE_DIR}")
configure_file(TtlConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/TtlConfig.cmake" @ONLY)

configure_file(TtlConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/TtlConfigVersion.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/TtlConfig.cmake"
  "${PROJECT_BINARY_DIR}/TtlConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev COMPONENT examples)

install(EXPORT TtlTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev COMPONENT examples)
