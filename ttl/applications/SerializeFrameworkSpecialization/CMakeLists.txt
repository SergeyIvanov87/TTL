#find_package(FreeGLUT REQUIRED)
#find_package(GLEW REQUIRED)

add_library(TtlAppsSerializeFramework INTERFACE)
target_include_directories(
  TtlAppsSerializeFramework
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
         )
target_link_libraries(TtlAppsSerializeFramework INTERFACE Ttl)

target_compile_definitions(
  TtlAppsSerializeFramework
  INTERFACE ${COMPILE_DEFS})

target_sources(TtlAppsSerializeFramework
                PUBLIC
                    FILE_SET serializeFrameworkSpecializationAppSources
                            TYPE HEADERS
                            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..
                        FILES
                            DeSerializeSpecificTypes.h
                            SerializeSpecificTypes.h
                            )

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(TtlAppsSerializeFramework INTERFACE --coverage -O0 -g)
    target_link_libraries(TtlAppsSerializeFramework INTERFACE --coverage)
endif()

# Install the files set of the target TtlAppsSerializeFramework into the appropriate directory
# and
# create an exported resource
install(TARGETS TtlAppsSerializeFramework
  EXPORT TtlAppsSerializeFrameworkTargets
  FILE_SET serializeFrameworkSpecializationAppSources DESTINATION ${INSTALL_INCLUDE_DIR}/applications)

# Create a cmake file describing this previously exported target TtlAppsSerializeFrameworkTargets
# and
# which can be used to import this target by an outside project by
# using the `include (TtlAppsSerializeFrameworkTargets.cmake)` mechanism
install(EXPORT TtlAppsSerializeFrameworkTargets
        FILE TtlAppsSerializeFrameworkTargets.cmake
        NAMESPACE Ttl::
        DESTINATION ${INSTALL_CMAKE_DIR}/TtlAppsSerializeFramework
)

# Create a cmake file describing a package configuration data
# which can be used to consume this library by an outside project by
# using the `find_package(Ttl)` mechanism
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/TtlAppsSerializeFrameworkConfig.cmake"
  INSTALL_DESTINATION ${INSTALL_CMAKE_DIR}/TtlAppsSerializeFramework
)

set(version 3.4.1)
set_property(TARGET TtlAppsSerializeFramework PROPERTY VERSION ${version})
set_property(TARGET TtlAppsSerializeFramework PROPERTY SOVERSION 3)
set_property(TARGET TtlAppsSerializeFramework PROPERTY
  INTERFACE_MathFunctions_MAJOR_VERSION 3)
set_property(TARGET TtlAppsSerializeFramework APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING TtlAppsSerializeFramework_MAJOR_VERSION
)

# Create a package configuration file for checking of compatibility of versions
# that package configuration file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/TtlAppsSerializeFrameworkConfigVersion.cmake"
  VERSION "${version}"
  COMPATIBILITY AnyNewerVersion
)

# Install  these package configurations files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TtlAppsSerializeFrameworkConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TtlAppsSerializeFrameworkConfigVersion.cmake"
        DESTINATION ${INSTALL_CMAKE_DIR}/TtlAppsSerializeFramework
)
