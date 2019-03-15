#ifndef IEVENTSPECIFICCONTROLLABLE_HPP
#define IEVENTSPECIFICCONTROLLABLE_HPP
#include "../ControllerEvent.h"
#include "IEventSpecificControllable.h"

#define T_ARGS_DECL         class ControllableImp, class RegisteredEvent
#define T_ARGS_DEF          ControllableImp, RegisteredEvent

template <T_ARGS_DECL>
constexpr ControlEventID ISpecificControllable<T_ARGS_DEF>::getRegisteredEventType()
{
    return RegisteredEvent::getControlEventID();
}

template <T_ARGS_DECL>
void ISpecificControllable<T_ARGS_DEF>::subscribeOnControlEvents(const EventCtrlDataToCommandStorageMapType &events)
{
    static bool firstCreationSubscription = true;
    if(firstCreationSubscription)
    {
        m_Event2ControlEventCommands = events;
        firstCreationSubscription = false;
    }
}

template <T_ARGS_DECL>
void ISpecificControllable<T_ARGS_DEF>::reSubscribeOnControlEvents(const EventCtrlDataToCommandStorageMapType &events)
{
    m_Event2ControlEventCommands = events;
}

//static interface
template <T_ARGS_DECL>
template <class ...Producer>
urc::ResultDescription ISpecificControllable<T_ARGS_DEF>::onSpecificProcessEvent(ProcessingEventType &specificEvent, Producer &&...producer)
{
    //check filter event by id
    auto it = m_Event2ControlEventCommands.data().find(specificEvent.getEventTypeSubscriptionData());
    if(it == m_Event2ControlEventCommands.data().end())
    {
        //skip
        return urc::ResultDescription();
    }

    Implementation &impl = static_cast<Implementation &>(*this);
    //process event call for ControllableImp

    return impl.template processSpecificEvent(specificEvent, it->second, std::forward<Producer>(producer)...);
}

template <T_ARGS_DECL>
template <class ...AllEventTypes>
const typename ISpecificControllable<T_ARGS_DEF>::ProcessingEventType &
    ISpecificControllable<T_ARGS_DEF>::getSpecificEvent(const CommonControllerEvent<AllEventTypes...> &event)
{
    return *(std::get<ProcessingEventTypeUniquePtr>(event.m_eventHolder));
}

template <T_ARGS_DECL>
template <class ...AllEventTypes>
typename ISpecificControllable<T_ARGS_DEF>::ProcessingEventType &
        ISpecificControllable<T_ARGS_DEF>::getSpecificEvent(CommonControllerEvent<AllEventTypes...> &event)
{
    return *(std::get<ProcessingEventTypeUniquePtr>(event.m_eventHolder));
}

#undef T_ARGS_DECL
#undef T_ARGS_DEF
#endif
