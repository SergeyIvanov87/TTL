#ifndef CONTROLLER_EVENT_CMDS_SPECIFIC_DECLARATION_H
#define CONTROLLER_EVENT_CMDS_SPECIFIC_DECLARATION_H

enum class ControlEventCMD
{
    EMPTY,
    MOVE_FORWARD,
    MODE_BACKWARD,
    STRAFE_LEFT,
    STRAFE_RIGHT,

    LOOK,
    CREATE_GAME_OBJECT,
    MAX_CONTROL_EVENT_TYPE
};

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (const T &lhs, ControlEventCMD rhs)
{
    return lhs != static_cast<T>(rhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (ControlEventCMD lhs, const T &rhs)
{
    return !(rhs != lhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (ControlEventCMD rhs, const T &lhs)
{
    return lhs != static_cast<T>(rhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (const T &rhs, ControlEventCMD lhs)
{
    return !(rhs != lhs);
}

inline constexpr const char *
    ControlEventCommands2String(ControlEventCMD ceventType)
{
    constexpr const char *strings[] = {
                    "EMPTY",
                    "MOVE_FORWARD",
                    "MOVE_BACKWARD",
                    "STRAFE_LEFT",
                    "STRAFE_RIGHT",

                   /* "LOOK_UP",
                    "LOOK_DOWN",
                    "LOOK_LEFT",
                    "LOOK_RIGHT",
                    */
                    "LOOK",
                    "CREATE_GAME_OBJECT"};
    static_assert(sizeof(strings)/sizeof(strings[0]) == ControlEventCMD::MAX_CONTROL_EVENT_TYPE);
    return strings[static_cast<int>(ceventType)];
}

inline ControlEventCMD
    string2ControlEventCommands(const std::string &ceventString)
{
    constexpr const char *strings[] = {
                    "EMPTY",
                    "MOVE_FORWARD",
                    "MOVE_BACKWARD",
                    "STRAFE_LEFT",
                    "STRAFE_RIGHT",

                    /*"LOOK_UP",
                    "LOOK_DOWN",
                    "LOOK_LEFT",
                    "LOOK_RIGHT",
                    */
                    "LOOK",
                    "CREATE_GAME_OBJECT"};
    static_assert(sizeof(strings)/sizeof(strings[0]) == ControlEventCMD::MAX_CONTROL_EVENT_TYPE);

    auto it = std::find(std::begin(strings), std::end(strings), ceventString);
    if(it == std::end(strings))
    {
        return ControlEventCMD::EMPTY;
    }
    return static_cast<ControlEventCMD>(std::distance(std::begin(strings), it));
}
#endif //CONTROLLER_EVENT_CMDS_SPECIFIC_DECLARATION_H
