#ifndef ICONTROLLABLE
#define ICONTROLLABLE

#include <functional>
#include "Framework/Utils/UnifiedResultCodes.h"
#include "IEventSpecificControllable.h"
#include "Framework/Utils/Visitor.h"
#include "Framework/Utils/LogTracerAdapter.h"

template<class ...AllEventTypes>
class CommonControllerEvent;

/*
 * All event type processor static interface
 * Used for routing especially registered event types to ISpecificControllable,
 * which filters it and route to ControllableImp class
 */
 template <class ControllableImp, class ...RegisteredEvents>
class IControllable :
                    public ISpecificControllable<ControllableImp, RegisteredEvents>...,
                    public Visitor<
                                    IControllable<ControllableImp,RegisteredEvents...>
                                    >
{
    typedef std::tuple<ISpecificControllable<ControllableImp, RegisteredEvents>...> SpecificControllables;
public:
    ~IControllable() = default;

    using ControlEventMultiTypesStorage = ControlEventMultiTypesStorageConfiguration<RegisteredEvents...>;
    using RegisteredObserverEventTypes = std::array<ControlEventID, sizeof...(RegisteredEvents)>;

    template <class ResourceProvider, class UsedTracer = Tracer<EmptyTracerImpl>>
    void loadControlEvents(ResourceProvider &provider, UsedTracer tracer = UsedTracer());
    //Filter event by registered type and route subscription into base classes
    void subscribeOnControlEvents(const ControlEventMultiTypesStorage &events);
    void reSubscribeOnControlEvents(const ControlEventMultiTypesStorage &events);
    RegisteredObserverEventTypes getRegisteredControlEvents() const;


    //Filter event by registered type and route processing into base classes
    template <class ...AllEventTypes>
    urc::ResultDescription onProcessEventDispatcher(CommonControllerEvent<AllEventTypes...> &event, bool notFilteredEvent = true);

    template <class ControllerImpl, class ...AllEventTypes>
    urc::ResultDescription onProcessEventDispatcher(ControllerImpl &producer, CommonControllerEvent<AllEventTypes...> &event, bool notFilteredEvent = true);

    template <class ControllerImpl, class ...AllEventTypes>
    urc::ResultDescription onProcessEventDispatcher(const ControllerImpl &producer, CommonControllerEvent<AllEventTypes...> &event, bool notFilteredEvent = true);

private:
    IControllable() = default;
    friend ControllableImp;
};

#endif
