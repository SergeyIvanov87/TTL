#ifndef CONTROLLER_EVENT_SPECIFIC_DECL_H
#define CONTROLLER_EVENT_SPECIFIC_DECL_H

#include "Framework/Base/EventFramework/ControllerEvent.hpp"

//Specific event type Declaration
using ObserverEvent = CommonControllerEvent<
                                            MouseEvent,
                                            KeyboardEvent
                                            /*TODO new ID here*/>;

//Export specific type to EventFramework
using ObserverEventUPtr = std::unique_ptr<ObserverEvent>;
using ControllerEvent = ObserverEventUPtr;


//Specific type creation Factory method
template<class T,  class ...Params>
inline ControllerEvent createControllerEvent(Params &&...args)
{
    return std::make_unique<ObserverEvent>( std::make_unique<T>(std::forward<Params>(args)...) );
}
#endif //CONTROLLER_EVENT_SPECIFIC_DECL_H
