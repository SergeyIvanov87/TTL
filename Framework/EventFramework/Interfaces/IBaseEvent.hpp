#ifndef BASE_EVENT_HPP
#define BASE_EVENT_HPP
#include <cassert>
#include "IBaseEvent.h"
//#include "Framework/Utils/Utils.h"
//#include "FrameworkSpecializations/EventFrameworkSpecialization/EventIdsSpecific.h"

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
            return EventTypeCtrlData(String2EventId(*dataList.begin()),
                                    getEventModifierDefault(),
                                    getEventIdStateDefault());
        case 2:
            {
                auto first = dataList.begin();
                auto second = std::next(first);
            return EventTypeCtrlData(String2EventId(*first),
                                    String2KeyModifier(*second),
                                    getEventIdStateDefault());
            }
        case 3:
        {
            auto first = dataList.begin();
            auto second = std::next(first);
            auto third = std::next(second);
            return EventTypeCtrlData(String2EventId(*first),
                                    String2KeyModifier(*second),
                                    String2EventIdState(*third));
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
            return EventTypeSubscriptionData(String2EventId(*dataList.begin()),
                                    getEventModifierDefault());
        case 2:
            {
                auto first = dataList.begin();
                auto second = std::next(first);
                return EventTypeSubscriptionData(String2EventId(*first),
                                    String2KeyModifier(*second));
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
_EventId IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::String2EventId(const std::string &eventIdStr)
{
    return Implementation::String2EventIdImpl(eventIdStr);
}

template <TEMPLATE_ARGS_LIST_DECL>
constexpr const char* IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventId2String(EventId eventId)
{
    return Implementation::EventId2StringImpl(eventId);
}

template <TEMPLATE_ARGS_LIST_DECL>
constexpr _EventIDMod IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventModifierDefault()
{
    return Implementation::getEventModifierDefaultImpl();
}

template <TEMPLATE_ARGS_LIST_DECL>
_EventIDMod IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::String2KeyModifier(const std::string &keyMod)
{
    return Implementation::String2KeyModifierImpl(keyMod);
}

template <TEMPLATE_ARGS_LIST_DECL>
constexpr _EventIdState IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::getEventIdStateDefault()
{
    return Implementation::getEventIdStateDefaultImpl();
}

template <TEMPLATE_ARGS_LIST_DECL>
_EventIdState IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::String2EventIdState(const std::string &state)
{
    return Implementation::String2EventIdStateImpl(state);
}

template <TEMPLATE_ARGS_LIST_DECL>
constexpr const char* IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::EventIdState2String(EventIdState state)
{
    return Implementation::EventIdState2StringImpl(state);
}

template <TEMPLATE_ARGS_LIST_DECL>
typename IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::ControlEventCMD
    IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::String2ControlEventCommands(const std::string &commandStr)
{
    return Implementation::String2ControlEventCommandsImpl(commandStr);
}

template <TEMPLATE_ARGS_LIST_DECL>
constexpr const char *IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::ControlEventCommands2String(ControlEventCMD command)
{
    return Implementation::ControlEventCommands2StringImpl(command);
}

template <TEMPLATE_ARGS_LIST_DECL>
std::string IBaseEvent<TEMPLATE_ARGS_LIST_DEF>::toString() const
{
    return static_cast<const Implementation *>(this)->toStringImpl();
}

#undef TEMPLATE_ARGS_LIST_DECL
#undef TEMPLATE_ARGS_LIST_DEF

#endif //BASE_EVENT_HPP
