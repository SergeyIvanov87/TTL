cmake_minimum_required (VERSION 3.15)

set (BRANCH_ROOT ${CMAKE_SOURCE_DIR})
get_filename_component(BRANCH_ROOT ${BRANCH_ROOT} ABSOLUTE)

set (PROCESS_DIR ${CMAKE_SOURCE_DIR})
get_filename_component(PROCESS_DIR ${PROCESS_DIR} ABSOLUTE)

set (BRANCH_NAME "resourcefw_example")

#==============================================================================

#Common Directories
#set (TTL_INC ${CMAKE_SOURCE_DIR}/../..)
#set (DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

#Headers dir
#set (INC_DIRECTORIES ${DIRECTORIES}
#                     ${TTL_INC})

#Source dir
#set (SRC_DIRECTORIES ${DIRECTORIES})

#==============================================================================
#Compile flags
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O0 -g3 -fvar-tracking -MD -Wall -Wno-invalid-offsetof -std=c++1z")

#DEBUG option
list (APPEND DEFINES_ALL -DDEBUG)

#==============================================================================
# Libraries
find_library (LIB_M m)
list (APPEND LIBS ${LIB_M})
#==============================================================================
#list(REMOVE_DUPLICATES INC_DIRECTORIES)
#list(REMOVE_DUPLICATES SRC_DIRECTORIES)
#list(REMOVE_DUPLICATES DEFINES_ALL)


add_definitions(${DEFINES_ALL})
list(APPEND SRC_ALL test.cpp)

#include_directories(${INC_DIRECTORIES})
#include_directories(${3PP_INCLUDES_DIR})
#include_directories(${CMAKE_SOURCE_DIR})

foreach (_lib ${LIBS})
    message(${_lib})
endforeach()


#==============================================================================
# Setup project
set (PROC_NAME "resourcefw_example")
add_executable(${PROC_NAME} ${SRC_ALL})
set_target_properties(${PROC_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROC_NAME} Ttl ${LIBS})

execute_process(COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/ASSETS)
execute_process(COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/ASSETS/ResourceA)
execute_process(COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/ASSETS/ResourceB)
execute_process(COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/ASSETS/ResourceC)
