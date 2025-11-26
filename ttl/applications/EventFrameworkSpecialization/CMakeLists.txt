#find_package(FreeGLUT REQUIRED)
#find_package(GLEW REQUIRED)

add_library(EventFrameworkSpecializationApp INTERFACE)
target_include_directories(
  EventFrameworkSpecializationApp
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/EventsImpl>"
         "$<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}/applications/EventFrameworkSpecialization>"
         )
target_link_libraries(EventFrameworkSpecializationApp INTERFACE Ttl)

target_compile_definitions(
  EventFrameworkSpecializationApp
  INTERFACE ${COMPILE_DEFS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(EventFrameworkSpecializationApp INTERFACE --coverage -O0 -g)
    target_link_libraries(EventFrameworkSpecializationApp INTERFACE --coverage)
endif()

install(TARGETS EventFrameworkSpecializationApp
  EXPORT EventFrameworkSpecializationApp
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)


list(APPEND TTL_APPS_TARGET EventFrameworkSpecializationApp)
set(TTL_APPS_TARGET ${TTL_APPS_TARGET} PARENT_SCOPE)
