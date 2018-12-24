#ifndef CONTROLLER_EVENT_IDS_SPECIFIC_DECLARATION_H
#define CONTROLLER_EVENT_IDS_SPECIFIC_DECLARATION_H
#include <string>
#include <algorithm>

//TODO - remove this ControlEventID - make specific
typedef enum ControlEventID : int
{
    //NULL_EVENT = 0,
    MOUSE_EVENT,
    KEYBOARD_EVENT,
    LOGIC_EVENT,
    TEST_EVENT,
    MAX_OBSERVER_EVENT
} ControlEventID;

inline constexpr const char *getObserverEventTypeString(ControlEventID type)
{
    constexpr const char *strings[] = {
        //"NULL_EVENT",
        "MOUSE_EVENT",
        "KEYBOARD_EVENT",
        "LOGIC_EVENT",
        "TEST_EVENT",
        "MAX_OBSERVER_EVENT"};
    static_assert(sizeof(strings)/sizeof(strings[0]) - 1 == ControlEventID::MAX_OBSERVER_EVENT, "ControlEventID and its descriptions are differ");
    return strings[(type > MAX_OBSERVER_EVENT ? MAX_OBSERVER_EVENT : type)];
}
#endif //CONTROLLER_EVENT_IDS_SPECIFIC_DECLARATION_H
