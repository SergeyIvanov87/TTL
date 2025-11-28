target_include_directories(
  Ttl
  INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/Interfaces>"
         "$<INSTALL_INTERFACE:include/ttl/EventFramework/Interfaces>"
         )


target_sources(Ttl
                PUBLIC
                    FILE_SET ttlEventFrameworkHeaders
                            TYPE HEADERS
                            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
                    FILES
                            Interfaces/IBaseEvent.h
                            Interfaces/IBaseEvent.hpp
                            Interfaces/IControllable.h
                            Interfaces/IControllable.hpp
                            Interfaces/IController.h
                            Interfaces/IEventConfigurator.h
                            Interfaces/IEventConfigurator.hpp
                            Interfaces/IEventConsumer.h
                            Interfaces/IEventConsumer.hpp
                            Interfaces/IEventSpecificControllable.h
                            Interfaces/IEventSpecificControllable.hpp
                            Interfaces/INotControllable.h
                            Interfaces/INotControllable.hpp
                            Interfaces/IEventFields.h
                            ControllerEvent.h
                            ControllerEvent.hpp
                            EventBroker.h
                            EventBroker.hpp
                            EventDirector.h
                            EventDirector.hpp
                            EventFramework.h
                            EventTypeTraitsDeclaration.h
                )
