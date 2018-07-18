#ifndef IEVENTCONFIGURATOR_H
#define IEVENTCONFIGURATOR_H
#include <unordered_set>

template <class Impl>
struct IEventConfigurator
/*,
                           public VisitorAcceptor<IEventConfigurator<Impl>>*/
{
    static constexpr const char *configurationControllableEvents = "ControllableEvents";

    template <class EventType, class UsedTracer>
    EventCtrlDataToCommandStorage<EventType> getControllerEventsByType(UsedTracer tracer) const;

    template <class ...EventTypes, class UsedTracer>
    ControlEventMultiTypesStorageConfiguration<EventTypes...> getControllerEventsByMultiTypes(UsedTracer tracer) const;

    template <class ...EventTypes, class UsedTracer>
    void fillControllerEventsByMultiTypes(ControlEventMultiTypesStorageConfiguration<EventTypes...> &in, UsedTracer tracer) const;

    //Interfaces
    template <class ConfigurationValueType>
    const ConfigurationValueType &getValueFromConfiguration(const std::string &configurationName, const std::string &valueName) const;
    template <class ConfigurationValueType>
    std::unordered_set<ConfigurationValueType> getMultipleValuesFromConfiguration(const std::string &configurationName, const std::string &valueName) const;
/*
    //VisitorAcceptor Interface
    template<class Visitor, class ...VisitorData>
    void acceptImpl(Visitor *visitor, VisitorData ...args) const
    {
          visitor->visit(this);
    }
*/
};

#endif //IEVENTCONFIGURATOR_H
