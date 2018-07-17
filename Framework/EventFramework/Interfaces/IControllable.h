#ifndef ICONTROLLABLE
#define ICONTROLLABLE

#include <functional>
#include "Framework/Utils/UnifiedResultCodes.h"
#include "IEventSpecificControllable.h"
#include "Framework/Utils/CTimeUtils.h"
#include "Framework/Utils/Visitor.h"


namespace Resources
{
    class ModelFileDescription;
}
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
    IControllable() = default;
    ~IControllable() = default;

    using ControlEventMultiTypesStorage = ControlEventMultiTypesStorageConfiguration<RegisteredEvents...>;
    using RegisteredObserverEventTypes = std::array<ControlEventID, sizeof...(RegisteredEvents)>;

    template <class ResourceProvider>
    void loadControlEvents(ResourceProvider &provider);
    //Filter event by registered type and route subscription into base classes
    void subscribeOnControlEvents(const ControlEventMultiTypesStorage &events);
    void reSubscribeOnControlEvents(const ControlEventMultiTypesStorage &events);
    RegisteredObserverEventTypes getRegisteredControlEvents() const;

    //Filter event by registered type and route processing into base classes
    urc::ResultDescription onProcessEventDispatcher(ObserverEvent &event, bool notFilteredEvent = true);



    //Visitor interface
    void visitImpl(const Resources::ModelFileDescription *visitedObjectModelDescription);
};
#endif
