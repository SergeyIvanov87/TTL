list (APPEND EXAMPLE_TARGETS eventfw_example
                             serializefw_example
                             serializefw_example_serialize)


add_subdirectory(EventFramework)
#add_subdirectory(ResourcesFramework)
add_subdirectory(SerializeFramework)

set (EXAMPLE_TARGETS_EXPORT ${EXAMPLE_TARGETS} PARENT_SCOPE)
add_custom_target(examples DEPENDS ${EXAMPLE_TARGETS})

install(TARGETS ${EXAMPLE_TARGETS}
        EXPORT TtlTargets
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
