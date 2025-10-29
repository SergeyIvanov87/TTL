#ifndef I_EVENT_CONSUMER_H
#define I_EVENT_CONSUMER_H

#include <ttl/include/Utils/UnifiedResultCodes.h>
#include "IEventSpecificControllable.h"


 template <class ControllableImp, class ...RegisteredEvents>
class IEventConsumer :
                    public ISpecificControllable<ControllableImp, RegisteredEvents>...
{
    typedef std::tuple<ISpecificControllable<ControllableImp, RegisteredEvents>...> SpecificControllables;
public:
    ~IEventConsumer() = default;

    using ControlEventMultiTypesStorage = ControlEventMultiTypesStorageConfiguration<RegisteredEvents...>;
    using RegisteredObserverEventTypes = std::array<ControlEventID, sizeof...(RegisteredEvents)>;

    template <class ResourceProvider, class UsedTracer = Tracer<EmptyTracerImpl>>
    void loadControlEvents(ResourceProvider &provider, UsedTracer tracer = UsedTracer());
    //Filter event by registered type and route subscription into base classes
    void subscribeOnControlEvents(const ControlEventMultiTypesStorage &events);
    void reSubscribeOnControlEvents(const ControlEventMultiTypesStorage &events);
    RegisteredObserverEventTypes getRegisteredControlEvents() const;


    //Filter event by registered type and route processing into base classes
    template <class Event>
    urc::ResultDescription onProcessEventDispatcher(Event &&event);

    template <class ControllerImpl, class Event>
    urc::ResultDescription onProcessEventDispatcher(ControllerImpl &producer, Event &&event);

    template <class ControllerImpl, class Event>
    urc::ResultDescription onProcessEventDispatcher(const ControllerImpl &producer, Event &&event);

private:
    IEventConsumer() = default;
    friend ControllableImp;
};

#endif  //I_EVENT_CONSUMER_H
