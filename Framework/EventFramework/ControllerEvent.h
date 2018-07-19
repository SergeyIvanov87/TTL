#ifndef CONTROLLER_EVENT_H
#define CONTROLLER_EVENT_H

#include <variant>
#include <memory>
#include "Framework/Utils/CTimeUtils.h"
#include "EventTypeTraitsDeclaration.h"

//Common template controller event, for all registered ControlEventID ID
template<class ...EventTypes>
struct CommonControllerEvent
{
    //variant stores all registered ControlEventID appropriated classes, using traits
    using EventTypeHolderList = std::tuple<
                                            //std::nullptr_t,
                                            typename EventTypeTraits<EventTypes>::EventTypeUniquePtr ...>;
    using EventTypeHolderImpl = std::variant<
                                            //std::nullptr_t,
                                            typename EventTypeTraits<EventTypes>::EventTypeUniquePtr ...>;

    CommonControllerEvent() = default;
    template<class SpecificEventPtr, typename = std::enable_if_t
                                                    <
                                                        std::is_constructible
                                                            <
                                                                EventTypeHolderImpl, SpecificEventPtr
                                                            >::value
                                                    >
            >
    CommonControllerEvent(SpecificEventPtr &&eventPtr) :
        m_eventHolder(std::forward<SpecificEventPtr>(eventPtr))
    {
    }

    CommonControllerEvent(CommonControllerEvent &&eventPtr) = default;

    ControlEventID getEventType() const;

    EventTypeHolderImpl m_eventHolder;
   /* int getButton() const
    {
        switch(m_eventHolder.index())
        {
            case ControlEventID::MOUSE_EVENT:
                return static_cast<int>(std::get<ControlEventID::MOUSE_EVENT>(m_eventHolder)->getEventTypeCtrlId());
            case ControlEventID::KEYBOARD_EVENT:
                return static_cast<int>(std::get<ControlEventID::KEYBOARD_EVENT>(m_eventHolder)->getEventTypeCtrlId().key);
            default:
                //TODO
                assert(false);
        }
    }*/
};
#endif //CONTROLLER_EVENT_H
