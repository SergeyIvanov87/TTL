#ifndef I_EVENT_CONSUMER_HPP
#define I_EVENT_CONSUMER_HPP

#include <functional>
#include <type_traits>
#include "Framework/Utils/LogTracerAdapter.h"
#include "IEventConsumer.h"
#include "Framework/Utils/CTimeUtils.h"
#include "IEventSpecificControllable.hpp"

#define T_ARGS_DECL      class ControllableImp, class ...RegisteredEvents
#define T_ARGS_DEF       ControllableImp, RegisteredEvents...

template<typename T>
using Remove_ref_cv = typename std::remove_reference<typename std::remove_cv<T>::type>::type ;

template <T_ARGS_DECL>
template <class ResourceProvider, class UsedTracer>
void
    IEventConsumer<T_ARGS_DEF>::loadControlEvents( ResourceProvider &provider, UsedTracer tracer)
{
    ControlEventMultiTypesStorage ret;
    provider.fillControllerEventsByMultiTypes(ret, tracer);
    subscribeOnControlEvents(ret);
}

template <T_ARGS_DECL>
void
    IEventConsumer<T_ARGS_DEF>::subscribeOnControlEvents(const ControlEventMultiTypesStorage &events)
{
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &events](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //dispatch processing
        ElementType::subscribeOnControlEvents(
                    std::get<Traits::index>(events));
    });
}

template <T_ARGS_DECL>
void
    IEventConsumer<T_ARGS_DEF>::reSubscribeOnControlEvents(const ControlEventMultiTypesStorage &events)
{
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &events](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //dispatch processing
        ElementType::subscribeOnControlEvents(
                    std::get<Traits::index>(events));
    });
}

template <T_ARGS_DECL>
typename IEventConsumer<T_ARGS_DEF>::RegisteredObserverEventTypes
    IEventConsumer<T_ARGS_DEF>::getRegisteredControlEvents() const
{
    RegisteredObserverEventTypes registeredEventTypes;
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &registeredEventTypes](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //set event registered
        registeredEventTypes[Traits::index] = ElementType::getRegisteredEventType();
    });
    return registeredEventTypes;
}

template <T_ARGS_DECL>
template <class Event>
urc::ResultDescription
    IEventConsumer<T_ARGS_DEF>::onProcessEventDispatcher(Event &&event)
{
    urc::ResultDescription retCode;
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &retCode, &event](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //Check event registered
        if constexpr(std::is_same_v<Remove_ref_cv<Event>, typename ElementType::ProcessingEventType>)
        {
            //dispatch processing
            retCode = ElementType::onSpecificProcessEvent(event);
        }
    });
    return retCode;
}

template <T_ARGS_DECL>
template <class ControllerImpl, class Event>
urc::ResultDescription
    IEventConsumer<T_ARGS_DEF>::onProcessEventDispatcher(ControllerImpl &producer,
                                                        Event &&event)
{
    static_assert(!std::is_base_of<ControllerImpl, IController<ControllerImpl>>(), "onProcessEventDispatcher - ControllerImpl is not derived from IController");
    urc::ResultDescription retCode;
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &retCode, &event, &producer](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //Check event registered
        if constexpr(std::is_same_v<Remove_ref_cv<Event>, typename ElementType::ProcessingEventType>)
        {
            //dispatch processing
            retCode = ElementType::onSpecificProcessEvent(event, producer);
        }
    });
    return retCode;
}

template <T_ARGS_DECL>
template <class ControllerImpl, class Event>
urc::ResultDescription
    IEventConsumer<T_ARGS_DEF>::onProcessEventDispatcher(const ControllerImpl &producer,
                                                       Event &&event)
{
    static_assert(!std::is_base_of<ControllerImpl, IController<ControllerImpl>>(), "onProcessEventDispatcher - ControllerImpl is not derived from IController");
    urc::ResultDescription retCode;
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &retCode, &event, &producer](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //Check event registered
        if constexpr(std::is_same_v<Remove_ref_cv<Event>, typename ElementType::ProcessingEventType>)
        {
            //dispatch processing
            retCode = ElementType::onSpecificProcessEvent(event, producer);
        }
    });
    return retCode;
}

#undef T_ARGS_DECL
#undef T_ARGS_DEF
#endif //I_EVENT_CONSUMER_HPP
