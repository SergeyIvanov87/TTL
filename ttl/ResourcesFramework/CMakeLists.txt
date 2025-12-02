target_include_directories(
  Ttl
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
         "$<INSTALL_INTERFACE:include>"
         )


target_sources(Ttl
                PUBLIC
                    FILE_SET ttlResourceFrameworkHeaders
                            TYPE HEADERS
                            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                    FILES
                            BaseObjectLoader.h
                            BaseObjectLoader.hpp
                            IBaseResource.h
                            LoadedResourcesHolder.h
                            LoadedResourcesHolder.hpp
                            ResourceTraits.h
                            ResourcesFramework.h
                            ResourcesFramework.hpp

                )
