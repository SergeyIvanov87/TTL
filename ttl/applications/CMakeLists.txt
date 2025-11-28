add_subdirectory(EventFrameworkSpecialization)

#add_library(TtlApps INTERFACE)
#target_link_libraries(TtlApps INTERFACE ${TTL_APPS_TARGET})
#
#target_include_directories(
#  TtlApps
#  INTERFACE
#        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
#         "$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}/applications>"
#         )
#
#
#set (TTL_APPS_TARGET ${TTL_APPS_TARGET} PARENT_SCOPE)
#
#include(CMakePackageConfigHelpers)
#configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
#  "${CMAKE_CURRENT_BINARY_DIR}/TTLAppsConfig.cmake"
#  INSTALL_DESTINATION ${INSTALL_CMAKE_DIR}/TtlApps
#)
#install(FILES
#        "${CMAKE_CURRENT_BINARY_DIR}/TTLAppsConfig.cmake"
#        "${CMAKE_CURRENT_BINARY_DIR}/TTLAppsConfig.cmake"
#    DESTINATION ${INSTALL_CMAKE_DIR}/TtlApps
#)
