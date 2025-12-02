#find_package(FreeGLUT REQUIRED)
#find_package(GLEW REQUIRED)

add_library(TtlAppsEventFramework INTERFACE)
target_include_directories(
  TtlAppsEventFramework
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/EventsImpl>"
         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
         )
target_link_libraries(TtlAppsEventFramework INTERFACE Ttl)

target_compile_definitions(
  TtlAppsEventFramework
  INTERFACE ${COMPILE_DEFS})

target_sources(TtlAppsEventFramework
                PUBLIC
                    FILE_SET eventFrameworkSpecializationAppSources
                            TYPE HEADERS
                            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
                        FILES
                            EventIDsDeclaration.h
                            EventsImpl/EventIdModifierSpecific.h
                            EventsImpl/KeyboardCommands.h
                            EventsImpl/KeyboardEvent.cpp
                            EventsImpl/KeyboardEvent.h
                            EventsImpl/MouseCommands.h
                            EventsImpl/MouseEvent.h
                            )

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(TtlAppsEventFramework INTERFACE --coverage -O0 -g)
    target_link_libraries(TtlAppsEventFramework INTERFACE --coverage)
endif()

# Install the files set of the target TtlAppsEventFramework into the appropriate directory
# and
# create an exported resource
install(TARGETS TtlAppsEventFramework
  EXPORT TtlAppsEventFrameworkTargets
  FILE_SET eventFrameworkSpecializationAppSources DESTINATION ${INSTALL_INCLUDE_DIR}/applications)

# Create a cmake file describing this previously exported target TtlAppsEventFrameworkTargets
# and
# which can be used to import this target by an outside project by
# using the `include (TtlAppsEventFrameworkTargets.cmake)` mechanism
install(EXPORT TtlAppsEventFrameworkTargets
        FILE TtlAppsEventFrameworkTargets.cmake
        NAMESPACE Ttl::
        DESTINATION ${INSTALL_CMAKE_DIR}/TtlAppsEventFramework
)

# Create a cmake file describing a package configuration data
# which can be used to consume this library by an outside project by
# using the `find_package(Ttl)` mechanism
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/TtlAppsEventFrameworkConfig.cmake"
  INSTALL_DESTINATION ${INSTALL_CMAKE_DIR}/TtlAppsEventFramework
)

set(version 3.4.1)
set_property(TARGET TtlAppsEventFramework PROPERTY VERSION ${version})
set_property(TARGET TtlAppsEventFramework PROPERTY SOVERSION 3)
set_property(TARGET TtlAppsEventFramework PROPERTY
  INTERFACE_MathFunctions_MAJOR_VERSION 3)
set_property(TARGET TtlAppsEventFramework APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING TtlAppsEventFrameworks_MAJOR_VERSION
)

# Create a package configuration file for checking of compatibility of versions
# that package configuration file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/TtlAppsEventFrameworkConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)

# Install  these package configurations files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TtlAppsEventFrameworkConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TtlAppsEventFrameworkConfigVersion.cmake"
        DESTINATION ${INSTALL_CMAKE_DIR}/TtlAppsEventFramework
)
