#ifndef ICONTROLLABLE
#define ICONTROLLABLE

#include <functional>
#include "../../Utils/ErrorCodes.h"
#include "IEventSpecificControllable.h"
#include "../../Utils/CTimeUtils.h"
#include "../../Resources/Visitor.h"


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
                    public Resources::Visitor<
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
    Errors::ErrorDescription onProcessEventDispatcher(ObserverEvent &event, bool notFilteredEvent = true);



    //Visitor interface
    void visitImpl(const Resources::ModelFileDescription *visitedObjectModelDescription);
};
#endif
