#ifndef ICONTROLLABLE_HPP
#define ICONTROLLABLE_HPP

#include "IControllable.h"
#include "Framework/Utils/CTimeUtils.h"
#include "IEventSpecificControllable.hpp"

#define T_ARGS_DECL      class ControllableImp, class ...RegisteredEvents
#define T_ARGS_DEF       ControllableImp, RegisteredEvents...

template <T_ARGS_DECL>
template <class ResourceProvider, class UsedTracer>
void
    IControllable<T_ARGS_DEF>::loadControlEvents( ResourceProvider &provider, UsedTracer tracer)
{
    ControlEventMultiTypesStorage ret;
    provider.fillControllerEventsByMultiTypes(ret, tracer);
    subscribeOnControlEvents(ret);
}
/*
template <T_ARGS_DECL>
void
    IControllable<T_ARGS_DEF>::visitImpl(const Resources::ModelFileDescription *visitedObjectModelDescription)
{
    //TODO remove control events code from ModelFileDescription to here
    ControlEventMultiTypesStorage ret;
    visitedObjectModelDescription->fillControllerEventsByMultiTypes(ret);
    subscribeOnControlEvents(ret);
}
*/

template <T_ARGS_DECL>
void
    IControllable<T_ARGS_DEF>::subscribeOnControlEvents(const ControlEventMultiTypesStorage &events)
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
    IControllable<T_ARGS_DEF>::reSubscribeOnControlEvents(const ControlEventMultiTypesStorage &events)
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
typename IControllable<T_ARGS_DEF>::RegisteredObserverEventTypes
    IControllable<T_ARGS_DEF>::getRegisteredControlEvents() const
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
template <class ...AllEventTypes>
urc::ResultDescription
    IControllable<T_ARGS_DEF>::onProcessEventDispatcher(CommonControllerEvent<AllEventTypes...> &event, bool notFilteredEvent/* = true*/)
{
    ControlEventID eventType = event.getEventType();
    urc::ResultDescription retCode;
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &retCode, &event, eventType](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //Check event registered
        if (eventType == ElementType::getRegisteredEventType())
        {
            //dispatch processing
            retCode = ElementType::onSpecificProcessEvent(ElementType::getSpecificEvent(event));
        }
    });
    return retCode;
}

template <T_ARGS_DECL>
template <class ControllerImpl, class ...AllEventTypes>
urc::ResultDescription
    IControllable<T_ARGS_DEF>::onProcessEventDispatcher(ControllerImpl &producer,
                                                        CommonControllerEvent<AllEventTypes...> &event,
                                                        bool notFilteredEvent/* = true*/)
{
    static_assert(!std::is_base_of<ControllerImpl, IController<ControllerImpl>>(), "onProcessEventDispatcher - ControllerImpl is not derived from IController");
    ControlEventID eventType = event.getEventType();
    urc::ResultDescription retCode;
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &retCode, &event, eventType, &producer](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //Check event registered
        if (eventType == ElementType::getRegisteredEventType())
        {
            //dispatch processing
            retCode = ElementType::onSpecificProcessEvent(ElementType::getSpecificEvent(event), producer);
        }
    });
    return retCode;
}

template <T_ARGS_DECL>
template <class ControllerImpl, class ...AllEventTypes>
urc::ResultDescription
    IControllable<T_ARGS_DEF>::onProcessEventDispatcher(const ControllerImpl &producer,
                                                        CommonControllerEvent<AllEventTypes...> &event,
                                                        bool notFilteredEvent/* = true*/)
{
    static_assert(!std::is_base_of<ControllerImpl, IController<ControllerImpl>>(), "onProcessEventDispatcher - ControllerImpl is not derived from IController");
    ControlEventID eventType = event.getEventType();
    urc::ResultDescription retCode;
    CTimeUtils::for_each_type_in_tuple_traits<SpecificControllables>([this, &retCode, &event, eventType, &producer](auto &&typeTraitsInfo)
    {
        auto ret = typeTraitsInfo;
        using Traits = decltype(ret);
        using ElementType = typename Traits::type;

        //Check event registered
        if (eventType == ElementType::getRegisteredEventType())
        {
            //dispatch processing
            retCode = ElementType::onSpecificProcessEvent(ElementType::getSpecificEvent(event), producer);
        }
    });
    return retCode;
}

#undef T_ARGS_DECL
#undef T_ARGS_DEF
#endif //ICONTROLLABLE_HPP
