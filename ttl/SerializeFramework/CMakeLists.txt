target_sources(Ttl
                PUBLIC
                    FILE_SET ttlSerializeFrameworkHeaders
                            TYPE HEADERS
                            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                    FILES
                            Interfaces/ISerializable.h
                            SerializeUtilsCommon.h
                            SerializeUtilsCommon.hpp

                )
