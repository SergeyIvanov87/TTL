#define T_ARGS_DECL         class ControllableImp, class RegisteredEvent
#define T_ARGS_DEF          ControllableImp, RegisteredEvent

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
urc::ResultDescription ISpecificControllable<T_ARGS_DEF>::onSpecificProcessEvent(ProcessingEventType &specificEvent)
{
    //check filter event by id
    auto it = m_Event2ControlEventCommands.data().find(specificEvent.getEventTypeSubscriptionData());
    if(it == m_Event2ControlEventCommands.data().end())
    {
        //skip
        return urc::ResultDescription();
    }

    //process event call for ControllableImp
    return (static_cast<Implementation *>(this))->processSpecificEvent(specificEvent, it->second);
}

template <T_ARGS_DECL>
const typename ISpecificControllable<T_ARGS_DEF>::ProcessingEventType &
    ISpecificControllable<T_ARGS_DEF>::getSpecificEvent(const ObserverEvent &event)
{
    return *(std::get<ProcessingEventTypeUniquePtr>(event.m_eventHolder));
}

template <T_ARGS_DECL>
typename ISpecificControllable<T_ARGS_DEF>::ProcessingEventType &
        ISpecificControllable<T_ARGS_DEF>::getSpecificEvent(ObserverEvent &event)
{
    return *(std::get<ProcessingEventTypeUniquePtr>(event.m_eventHolder));
}

#undef T_ARGS_DECL
#undef T_ARGS_DEF
