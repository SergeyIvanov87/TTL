#ifndef BASE_EVENT_HPP
#define BASE_EVENT_HPP
#include <cassert>
#include "IBaseEvent.h"
#include "IEventFields.h"

//helpers
#define TEMPLATE_ARGS_LIST_DECL  class Implementation, class _EventId, class _EventIDMod, class _EventIdState, class _ControlEventCMD
#define TEMPLATE_ARGS_LIST_DEF   Implementation, _EventId, _EventIDMod, _EventIdState, _ControlEventCMD


template <TEMPLATE_ARGS_LIST_DECL>
template<class ...UTypes>
IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::IBaseEvent(UTypes&&... args) :
m_eventCtrlData(std::forward<UTypes>(args)...)
{
}

template <TEMPLATE_ARGS_LIST_DECL>
const _EventId &IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventTypeCtrlId() const
{
    return std::get<EventTypeCtrlDataEnum::BASE_EVENT_ID>(m_eventCtrlData);
}

template <TEMPLATE_ARGS_LIST_DECL>
const _EventIDMod &IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventTypeCtrlIdModifier() const
{
    return std::get<EventTypeCtrlDataEnum::BASE_EVENT_ID_MODIFIER>(m_eventCtrlData);
}

template <TEMPLATE_ARGS_LIST_DECL>
const _EventIdState &IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventTypeCtrlIdState() const
{
    return std::get<EventTypeCtrlDataEnum::BASE_EVENT_ID_STATE>(m_eventCtrlData);
}

template <TEMPLATE_ARGS_LIST_DECL>
const _EventId &IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventId(const typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeCtrlData &buttonInfo)
{
     return std::get<EventTypeCtrlDataEnum::BASE_EVENT_ID>(buttonInfo);
}

template <TEMPLATE_ARGS_LIST_DECL>
const _EventIDMod &IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventIdModifier(const typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeCtrlData &buttonInfo)
{
    return std::get<EventTypeCtrlDataEnum::BASE_EVENT_ID_MODIFIER>(buttonInfo);
}

template <TEMPLATE_ARGS_LIST_DECL>
const _EventIdState &IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventIdState(const typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeCtrlData &buttonInfo)
{
    return std::get<EventTypeCtrlDataEnum::BASE_EVENT_ID_STATE>(buttonInfo);
}

template <TEMPLATE_ARGS_LIST_DECL>
const typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeCtrlData &IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventTypeCtrlData() const
{
    return m_eventCtrlData;
}

template <TEMPLATE_ARGS_LIST_DECL>
typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeSubscriptionData IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventTypeSubscriptionData() const
{
    return std::make_tuple(getEventTypeCtrlId(), getEventTypeCtrlIdModifier());
}

template <TEMPLATE_ARGS_LIST_DECL>
typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeCtrlData IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::parseEventCtrlData(const std::list<std::string> &dataList)
{
    using EventTypeCtrlData = typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeCtrlData;
    switch(dataList.size())
    {
        case 1:
            return std::make_tuple(_EventId::createFromString(*dataList.begin()), _EventIDMod(), _EventIdState());
        case 2:
            {
                auto first = dataList.begin();
                auto second = std::next(first);
            return std::make_tuple(_EventId::createFromString(*first),
                                    _EventIDMod::createFromString(*second), _EventIdState());
            }
        case 3:
        {
            auto first = dataList.begin();
            auto second = std::next(first);
            auto third = std::next(second);
            return std::make_tuple(_EventId::createFromString(*first),
                                    _EventIDMod::createFromString(*second),
                                    _EventIdState::createFromString(*third));
        }
        default:
            assert(false);
    }
    return EventTypeCtrlData();
}

template <TEMPLATE_ARGS_LIST_DECL>
typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeSubscriptionData IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::parseEventSubscriptionData(const std::list<std::string> &dataList)
{
    using EventTypeSubscriptionData = typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventTypeSubscriptionData;
    switch(dataList.size())
    {
        case 1:
            return std::make_tuple(_EventId::createFromString(*dataList.begin()), _EventIDMod());
        case 2:
            {
                auto first = dataList.begin();
                auto second = std::next(first);
                return std::make_tuple(_EventId::createFromString(*first),
                                                 _EventIDMod::createFromString(*second));
            }
        default:
            assert(false);
    }
    return EventTypeSubscriptionData();
}





//Interface methods
template <TEMPLATE_ARGS_LIST_DECL>
constexpr const char *IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventTypeDescription()
{
    return Implementation::getEventTypeDescriptionImpl();
}

template <TEMPLATE_ARGS_LIST_DECL>
constexpr _EventIDMod IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventModifierDefault()
{
    return Implementation::getEventModifierDefaultImpl();
}

template <TEMPLATE_ARGS_LIST_DECL>
typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::ControlEventCMD
    IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::String2ControlEventCommands(const std::string &commandStr)
{
    return ControlEventCMD::createFromString(commandStr);
}

template <TEMPLATE_ARGS_LIST_DECL>
constexpr const char *IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::ControlEventCommands2String(ControlEventCMD command)
{
    return command.toCString();
}

template <TEMPLATE_ARGS_LIST_DECL>
std::string IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::toString() const
{
    return static_cast<const Implementation *>(this)->toStringImpl();
}

#undef TEMPLATE_ARGS_LIST_DECL
#undef TEMPLATE_ARGS_LIST_DEF

#endif //BASE_EVENT_HPP
