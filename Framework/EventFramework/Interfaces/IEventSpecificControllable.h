#ifndef IEVENTSPECIFICCONTROLLABLE
#define IEVENTSPECIFICCONTROLLABLE
#include <unordered_map>
#include <map>
#include "../EventTypeTraitsDeclaration.h"
//#include "FrameworkSpecializations/EventFrameworkSpecialization/EventFrameworkSpecializationExport.h"

/*
 * Specific event type processor static interface.
 * Route filtered event by id processing to ControllableImp class
 */
template<class ...AllEventTypes>
class CommonControllerEvent;


template <class ControllableImp, class RegisteredEvent>
class ISpecificControllable
{
    typedef ControllableImp Implementation;
public:
    ISpecificControllable() = default;
    ~ISpecificControllable() = default;

    //typedef
    using TypeTraits = EventTypeTraits<RegisteredEvent>;
    using ProcessingEventTraits = typename TypeTraits::InputEventTraits;
    using ProcessingEventType = typename TypeTraits::EventType;
    using ProcessingEventTypeUniquePtr = typename TypeTraits::EventTypeUniquePtr;
    using EventCtrlDataToCommandStorageMapType = EventCtrlDataToCommandStorage<RegisteredEvent>;

    //get static registered event type
    static constexpr ControlEventID getRegisteredEventType();

    //Configure filtered event for specific type
    static void subscribeOnControlEvents(const EventCtrlDataToCommandStorageMapType &event);
    static void reSubscribeOnControlEvents(const EventCtrlDataToCommandStorageMapType &event);

    //static interface
    urc::ResultDescription onSpecificProcessEvent(ProcessingEventType &event);

    template <class ...AllEventTypes>
    static const ProcessingEventType &getSpecificEvent(const CommonControllerEvent<AllEventTypes...> &event);

    template <class ...AllEventTypes>
    static ProcessingEventType &getSpecificEvent(CommonControllerEvent<AllEventTypes...> &event);
private:
    static EventCtrlDataToCommandStorageMapType m_Event2ControlEventCommands;
};

//static storage filter definition
template <class ControllableImp, class RegisteredEvent>
typename ISpecificControllable<ControllableImp, RegisteredEvent>::EventCtrlDataToCommandStorageMapType ISpecificControllable<ControllableImp, RegisteredEvent>::m_Event2ControlEventCommands;
#endif
