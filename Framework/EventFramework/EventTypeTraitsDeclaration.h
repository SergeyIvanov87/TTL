#ifndef INPUT_EVENT_TRAITS_LIST_DECLARATION_H
#define INPUT_EVENT_TRAITS_LIST_DECLARATION_H
#include <memory>
#include <map>
#include "FrameworkSpecializations/EventFrameworkSpecialization/EventIDsDeclaration.h"


//Traits: convert ControlEventType ID to class
template <class Event>
struct EventTypeTraits
{
    typedef Event EventType;
    typedef std::shared_ptr<EventType> EventTypeSharedPtr;
    typedef std::unique_ptr<EventType> EventTypeUniquePtr;
    using InputEventTraits = typename EventType::EventTypeCtrlData;
    using InputEventSubscriptionTraits = typename EventType::EventTypeSubscriptionData;
};

//Common storage for storing EventTypeCtrlData mapping on ControlEventCMD
//Implemented as template class, because it hold 'ControlEventID type'
//and can be used in template argument function deduction by 'ControlEventID type'
//instead of std::map<EventTraits<type>::EVentCtrl>
template <class Event>
struct EventCtrlDataToCommandStorage
{
    static constexpr ControlEventID
        getEventType() {return Event::getControlEventID();}

    typedef EventTypeTraits<Event> Traits;
    typedef typename Traits::EventType EventType;
    typedef typename EventType::EventTypeCtrlData EventTypeCtrlData;
    typedef typename EventType::EventTypeSubscriptionData EventTypeSubscriptionData;
    typedef typename EventType::ControlEventCMD ControlEventCMD;

    using EventCtrlDataToCommandStorageImpl = std::map<EventTypeSubscriptionData, ControlEventCMD>;

    EventCtrlDataToCommandStorageImpl &data() {return innerStorage; }
    const EventCtrlDataToCommandStorageImpl &data() const {return innerStorage; }

    EventCtrlDataToCommandStorageImpl innerStorage;
};

//Holds all registered storage for all event types
template<class ...Events>
using ControlEventMultiTypesStorageConfiguration =
                                std::tuple<
                                        EventCtrlDataToCommandStorage<Events> ...>;
#endif //INPUT_EVENT_TRAITS_LIST_DECLARATION_H
