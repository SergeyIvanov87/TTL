cmake_minimum_required (VERSION 3.23)

project(Ttl CXX)

set (BRANCH_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(BRANCH_ROOT ${BRANCH_ROOT} ABSOLUTE)

set (PROCESS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(PROCESS_DIR ${PROCESS_DIR} ABSOLUTE)

# set install_path's
set(INSTALL_LIB_DIR lib/ttl CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include/ttl CACHE PATH "Installation directory for header files")
set(INSTALL_EXAMPLE_DIR examples CACHE PATH "Installation directory for examples files")

if(WIN32 AND NOT CYGWIN)
message ("DEFAULT_INSTALL_CMAKE_DIR: ${DEFAULT_INSTALL_CMAKE_DIR}")
  set(DEFAULT_INSTALL_CMAKE_DIR CMake)
else()
  set(DEFAULT_INSTALL_CMAKE_DIR lib/cmake)
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
#-S- don't need binary from examples to install
#add_subdirectory(examples)
add_subdirectory(tests)

export(TARGETS Ttl #TtlApps Ttl ${EXAMPLE_TARGETS_EXPORT} ${TTL_APPS_TARGET}
       FILE "${PROJECT_BINARY_DIR}/Ttl.cmake")
export(PACKAGE Ttl)

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}/Ttl"
   "${INSTALL_INCLUDE_DIR}")

#-S- set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
#configure_file(TtlConfig.cmake.in
#  "${PROJECT_BINARY_DIR}/TtlConfig.cmake" @ONLY)

#-S- set(CONF_INCLUDE_DIRS "\${PROJECT_SOURCE_DIR}/${REL_INCLUDE_DIR}")
#configure_file(TtlConfig.cmake.in
#  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/TtlConfig.cmake" @ONLY)

#configure_file(TtlConfigVersion.cmake.in
#  "${PROJECT_BINARY_DIR}/TtlConfigVersion.cmake" @ONLY)

#-S- install(FILES
#  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/TtlConfig.cmake"
#  "${PROJECT_BINARY_DIR}/TtlConfigVersion.cmake"
#  DESTINATION "${INSTALL_CMAKE_DIR}/Ttl" COMPONENT dev COMPONENT examples)

install(TARGETS Ttl #TtlApps Ttl ${TTL_APPS_TARGET}
  EXPORT TtlTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  FILE_SET testSources DESTINATION "${INSTALL_EXAMPLE_DIR}" COMPONENT examples
  FILE_SET ttlEventFrameworkHeaders DESTINATION "${INSTALL_INCLUDE_DIR}/EventFramework" COMPONENT sources
  FILE_SET ttlResourceFrameworkHeaders DESTINATION "${INSTALL_INCLUDE_DIR}/ResourcesFramework" COMPONENT sources
  FILE_SET ttlSerializeFrameworkHeaders DESTINATION "${INSTALL_INCLUDE_DIR}/SerializeFramework" COMPONENT sources
  FILE_SET ttlUtilsHeaders DESTINATION "${INSTALL_INCLUDE_DIR}/Utils" COMPONENT sources
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
    COMPONENT dev)

#-S-install(DIRECTORY ${PROJECT_SOURCE_DIR}/ttl ${PROJECT_SOURCE_DIR}/ttl/applications DESTINATION "${INSTALL_INCLUDE_DIR}"
#-S-        FILES_MATCHING PATTERN "*.h*")

install(EXPORT TtlTargets
    FILE TtlTargets.cmake
    NAMESPACE Ttl::
    DESTINATION "${INSTALL_CMAKE_DIR}/Ttl" COMPONENT dev COMPONENT examples
)


# Create a cmake file describing a package configuration data
# which can be used to consume this library by an outside project by
# using the `find_package(Ttl)` mechanism
include(CMakePackageConfigHelpers)
configure_package_config_file( ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/TtlConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CMAKE_DIR}/Ttl
)

# configure version
set(TTL_MAJOR_VERSION 0)
set(TTL_MINOR_VERSION 1)
set(TTL_PATCH_VERSION 0)
set(TTL_VERSION
    ${TTL_MAJOR_VERSION}.${TTL_MINOR_VERSION}.${TTL_PATCH_VERSION})

set_property(TARGET Ttl PROPERTY VERSION ${TTL_VERSION})
set_property(TARGET Ttl PROPERTY SOVERSION ${TTL_MAJOR_VERSION})
set_property(TARGET Ttl PROPERTY TtlTargets_MAJOR_VERSION ${TTL_MAJOR_VERSION})
set_property(TARGET Ttl APPEND PROPERTY COMPATIBLE_INTERFACE_STRING TtlTargets_MAJOR_VERSION)

# Create a package configuration file for checking of compatibility of versions
# that package configuration file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/TtlConfigVersion.cmake"
  VERSION "${TTL_VERSION}"
  COMPATIBILITY AnyNewerVersion
)

# Install  these package configurations files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TtlConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TtlConfigVersion.cmake"
        DESTINATION ${INSTALL_CMAKE_DIR}/Ttl
)
