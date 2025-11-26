add_subdirectory(EventFrameworkSpecialization)

add_library(TtlApps INTERFACE)
target_link_libraries(TtlApps INTERFACE ${TTL_APPS_TARGET})

target_include_directories(
  TtlApps
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
         "$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}/applications>"
         )


set (TTL_APPS_TARGET ${TTL_APPS_TARGET} PARENT_SCOPE)
