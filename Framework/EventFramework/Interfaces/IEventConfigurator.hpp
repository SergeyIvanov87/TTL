#ifndef IEVENTCONFIGURATOR_HPP
#define IEVENTCONFIGURATOR_HPP
#include "IEventConfigurator.h"
#include <Framework/Utils/Utils.h>

#define T_ARGS_DECL         class Impl
#define T_ARGS_DEF          Impl
template <T_ARGS_DECL>
template <class ConfigurationValueType>
ConfigurationValueType
    IEventConfigurator<T_ARGS_DEF>::getValueFromConfiguration(const std::string &configurationName, const std::string &valueName) const
{
    return static_cast<const Impl *>(this)->template getValueFromConfigurationImpl<ConfigurationValueType>(configurationName, valueName);
}

template <T_ARGS_DECL>
template <class ConfigurationValueType>
std::unordered_set<ConfigurationValueType>
    IEventConfigurator<T_ARGS_DEF>::getMultipleValuesFromConfiguration(const std::string &configurationName, const std::string &valueName) const
{
    return static_cast<const Impl *>(this)->template getMultipleValuesFromConfigurationImpl<ConfigurationValueType>(configurationName, valueName);
}

template <T_ARGS_DECL>
template <class EventType, class UsedTracer>
EventCtrlDataToCommandStorage<EventType>
    IEventConfigurator<T_ARGS_DEF>::getControllerEventsByType(UsedTracer tracerWrapper) const
{
    using EventCtrl = typename EventCtrlDataToCommandStorage<EventType>::EventType;
    using EventSubscriptionData = typename EventCtrlDataToCommandStorage<EventType>::EventTypeSubscriptionData;
    using ReturnType = EventCtrlDataToCommandStorage<EventType>;

    constexpr const char *eventTypeStr = EventType::getEventTypeDescriptionImpl();
    tracerWrapper.trace(__FUNCTION__, ": ", eventTypeStr);

    ReturnType result;

    //find ControllableEvents, if object is controllable
    auto controllableEvents = getValueFromConfiguration<std::string>(configurationControllableEvents, configurationControllableEvents);
    if(controllableEvents.empty())
    {
        tracerWrapper.trace(configurationControllableEvents, " is missing");
        return result;
    }

    //Get all categories in ControlEventTypes
    std::list<std::string> controllableEventsList;
    if (!Utils::tokenizeString(controllableEvents, ',', controllableEventsList))
    {
        tracerWrapper.trace(configurationControllableEvents, " is empty");
        return result;
    }
    tracerWrapper.trace(configurationControllableEvents, ": ", controllableEvents);

    //iterate over controllable events types/category
    for(const auto &event : controllableEventsList)
    {
        auto eventCategoryTracer = tracerWrapper;
        eventCategoryTracer.trace("Check category: ", event);

        //filter out control event type
        if(event != eventTypeStr)
        {
            continue;
        }

        //read all events for specific control event type/category
        auto controllableEventsStr = getValueFromConfiguration<std::string>(configurationControllableEvents, event);
        eventCategoryTracer.trace("Category event commands: ", controllableEventsStr);

        //total events list
        std::list<std::string> controllableSubEventsList;
        if (!Utils::tokenizeString(controllableEventsStr, ',', controllableSubEventsList))
        {
            eventCategoryTracer.trace("Category event commands is empty list");
            continue;
        }

        //Get event ctl data for every event
        for(const auto &controlCommand : controllableSubEventsList)
        {
            /*
            Resources::ModelFileDescription::ValueCRef controlKeysListStr = getDescriptionValue(controlCommand);
            std::list<std::string> controlKeysList;
            if (!Utils::tokenizeString(controlKeysListStr, '+', controlKeysList))
            {
                LOG4CPLUS_ERROR(logger, LOG4CPLUS_TEXT("Empty keys list for control command: ") << controlCommand);
                continue;
            }

            LOG4CPLUS_DEBUG(logger, LOG4CPLUS_TEXT("controlCommand: ") << controlCommand);

            //use Event traits to parse EventCtrlData from string list
            EventSubscriptionData defaultCtlData = EventCtrl::parseEventSubscriptionData(controlKeysList);

            //Convert command from string to enum
            ControlEventCommands ceventType = string2ControlEventCommands(controlCommand);
            result.data().insert({defaultCtlData, ceventType});
            * */

            auto subCategoryEventTracer = eventCategoryTracer;
            subCategoryEventTracer.trace("ControlCommand: ", controlCommand);

            //TODO handle multiple values
            auto commands = getMultipleValuesFromConfiguration<std::string>(configurationControllableEvents, controlCommand);
            if(commands.empty())
            {
                subCategoryEventTracer.trace("ControlCommand: ", controlCommand, " not found");
                continue; //next command
            }
            for(auto it = commands.begin(); it != commands.end(); ++it)
            {
                auto controlKeysTracer = subCategoryEventTracer;
                controlKeysTracer.trace("ControlKey(s): ",  *it);

                std::list<std::string> controlKeysList;
                if (!Utils::tokenizeString(*it, '+', controlKeysList))
                {
                    controlKeysTracer.trace("Empty keys list for control command: ", controlCommand);
                    continue;
                }

                //use Event traits to parse EventCtrlData from string list
                EventSubscriptionData defaultCtlData = EventCtrl::parseEventSubscriptionData(controlKeysList);

                //Convert command from string to enum
                auto ceventType = EventCtrl::String2ControlEventCommands(controlCommand);
                result.data().insert({defaultCtlData, ceventType});
            }
        }
    }
    return result;
}


template<T_ARGS_DECL>
template <class  ...EventTypes, class UsedTracer>
ControlEventMultiTypesStorageConfiguration<EventTypes...>
    IEventConfigurator<T_ARGS_DEF>::getControllerEventsByMultiTypes(UsedTracer tracer) const
{
    tracer.trace(__FUNCTION__);
    return ControlEventMultiTypesStorageConfiguration<EventTypes...>(getControllerEventsByType<EventTypes, UsedTracer>(tracer)...);
}

template<T_ARGS_DECL>
template <class ...EventTypes, class UsedTracer>
void
    IEventConfigurator<T_ARGS_DEF>::fillControllerEventsByMultiTypes(ControlEventMultiTypesStorageConfiguration<EventTypes...> &in, UsedTracer tracer) const
{
    tracer.trace(__FUNCTION__);
    in = getControllerEventsByMultiTypes<EventTypes...>(tracer);
}
#undef T_ARGS_DEF
#undef T_ARGS_DECL
#endif //IEVENTCONFIGURATOR_HPP
