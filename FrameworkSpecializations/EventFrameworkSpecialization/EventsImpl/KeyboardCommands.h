#ifndef KEYBOARD_COMMANDS_H
#define KEYBOARD_COMMANDS_H

enum class KeyboardEventCMD
{
    EMPTY,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    STRAFE_LEFT,
    STRAFE_RIGHT,

    MAX_EVENT_TYPE
};

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (const T &lhs, KeyboardEventCMD rhs)
{
    return lhs != static_cast<T>(rhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (KeyboardEventCMD lhs, const T &rhs)
{
    return !(rhs != lhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (KeyboardEventCMD rhs, const T &lhs)
{
    return lhs != static_cast<T>(rhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (const T &rhs, KeyboardEventCMD lhs)
{
    return !(rhs != lhs);
}
#endif //KEYBOARD_COMMANDS_H
