#ifndef EVENT_FRAMEWORK_MAIN_H
#define EVENT_FRAMEWORK_MAIN_H
#include <type_traits>
#include "Framework/EventFramework/Interfaces/IControllable.h"
#include <Framework/EventFramework/Interfaces/IEventConsumer.h>
#include "Framework/EventFramework/Interfaces/IController.h"
#include "Framework/EventFramework/Interfaces/IEventConfigurator.hpp"
#include "Framework/EventFramework/ControllerEvent.hpp"

template <class ...RegisteredEventTypes>
struct EventFrameworkFactory
{
    //Specific event type Declaration
    using ObserverEvent = CommonControllerEvent<RegisteredEventTypes...>;

    //Export specific type to EventFramework
    using ObserverEventUPtr = std::unique_ptr<ObserverEvent>;
    using ControllerEvent = ObserverEventUPtr;

    //Specific type creation Factory method
    template<class T,  class ...Params>
    static inline ControllerEvent createControllerEvent(Params &&...args)
    {
        return std::make_unique<ObserverEvent>( std::make_unique<T>(std::forward<Params>(args)...) );
    }
};

#endif //#define EVENT_FRAMEWORK_MAIN_H
