cmake_minimum_required (VERSION 3.23)

set (PROC_NAME "serializefw_examples")
project(${PROC_NAME})

set (BRANCH_ROOT ${CMAKE_SOURCE_DIR})
get_filename_component(BRANCH_ROOT ${BRANCH_ROOT} ABSOLUTE)

set (PROCESS_DIR ${CMAKE_SOURCE_DIR})
get_filename_component(PROCESS_DIR ${PROCESS_DIR} ABSOLUTE)


#==============================================================================
#Compile flags
set (CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#==============================================================================
# Setup project
add_executable(serializefw_example test.cpp)
target_link_libraries(serializefw_example Ttl)

add_executable(serializefw_example_serialize test_serialize.cpp)
target_link_libraries(serializefw_example_serialize Ttl)


install(TARGETS serializefw_example serializefw_example_serialize
  EXPORT examples
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
