#ifndef BASE_EVENT_H
#define BASE_EVENT_H
#include <tuple>
#include <string>
#include <list>
#include <utility>


template <class Implementation, class _EventId, class _EventIDMod, class _EventIdState, class _ControlEventCMD>
class IBaseEvent
{
public:
    using EventType = Implementation;
    using EventId = _EventId;
    using EventIDMod = _EventIDMod;
    using EventIdState = _EventIdState;
    using ControlEventCMD = _ControlEventCMD;

    enum EventTypeCtrlDataEnum
    {
        BASE_EVENT_ID,
        BASE_EVENT_ID_MODIFIER,
        BASE_EVENT_ID_STATE,
    };
    using EventTypeCtrlData = std::tuple<
                                        EventId,
                                        EventIDMod,
                                        EventIdState>;
    using EventTypeSubscriptionData = std::tuple<
                                        EventId,
                                        EventIDMod>;

    template<class... UTypes>
    IBaseEvent(UTypes&&... args);
    ~IBaseEvent() = default;

    static EventTypeCtrlData
        parseEventCtrlData(const std::list<std::string> &dataList);

    static EventTypeSubscriptionData
        parseEventSubscriptionData(const std::list<std::string> &dataList);

    static const EventId &
        getEventId(const EventTypeCtrlData &buttonInfo);

    static const EventIDMod &
        getEventIdModifier(const EventTypeCtrlData &buttonInfo);

    static const EventIdState &
        getEventIdState(const EventTypeCtrlData &buttonInfo);

    const EventTypeCtrlData &
        getEventTypeCtrlData() const;

    EventTypeSubscriptionData
        getEventTypeSubscriptionData() const;

    const EventId &
        getEventTypeCtrlId() const;

    const EventIDMod &
        getEventTypeCtrlIdModifier() const;

    const EventIdState &
        getEventTypeCtrlIdState() const;

    //Interface methods
    //static getControlEventID();
    static constexpr const char * getEventTypeDescription();

    static EventId String2EventId(const std::string &eventIdStr);
    static constexpr const char* EventId2String(EventId eventId);

    static constexpr _EventIDMod getEventModifierDefault();
    static _EventIDMod String2KeyModifier(const std::string &keyMod);

    static constexpr _EventIdState getEventIdStateDefault();
    static EventIdState String2EventIdState(const std::string &state);
    static constexpr const char* EventIdState2String(EventIdState state);

    static ControlEventCMD String2ControlEventCommands(const std::string &commandStr);
    static constexpr const char *ControlEventCommands2String(ControlEventCMD command);
    std::string toString() const;

protected:
    EventTypeCtrlData m_eventCtrlData;
};

#endif
