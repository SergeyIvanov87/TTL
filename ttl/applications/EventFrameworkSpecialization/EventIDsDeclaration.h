#ifndef CONTROLLER_EVENT_IDS_SPECIFIC_DECLARATION_H
#define CONTROLLER_EVENT_IDS_SPECIFIC_DECLARATION_H
#include <string>
#include <algorithm>

//TODO - remove this ControlEventID - make specific
typedef enum EventFrameworkAppControlEventID : int
{
    //NULL_EVENT = 0,
    MOUSE_EVENT,
    KEYBOARD_EVENT,
    LOGIC_EVENT,
    USER_EVENT_BEGIN,
    MAX_OBSERVER_EVENT
} EventFrameworkAppControlEventID;

inline constexpr const char *getObserverEventTypeString(EventFrameworkAppControlEventID type)
{
    constexpr const char *strings[] = {
        //"NULL_EVENT",
        "MOUSE_EVENT",
        "KEYBOARD_EVENT",
        "LOGIC_EVENT",
        "USER_EVENT_BEGIN",
        "MAX_OBSERVER_EVENT"};
    static_assert(sizeof(strings)/sizeof(strings[0]) - 1 == EventFrameworkAppControlEventID::MAX_OBSERVER_EVENT, "EventFrameworkAppControlEventID and its descriptions are differ");
    return strings[(type > MAX_OBSERVER_EVENT ? MAX_OBSERVER_EVENT : type)];
}
#endif //CONTROLLER_EVENT_IDS_SPECIFIC_DECLARATION_H
