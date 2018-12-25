#ifndef MOUSE_COMMANDS_H
#define MOUSE_COMMANDS_H

enum class MouseEventCMD
{
    EMPTY,
    LOOK,
    MOUSE_BUTTON_EVENT,
    MAX_EVENT_TYPE
};

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (const T &lhs, MouseEventCMD rhs)
{
    return lhs != static_cast<T>(rhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (MouseEventCMD lhs, const T &rhs)
{
    return !(rhs != lhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (MouseEventCMD rhs, const T &lhs)
{
    return lhs != static_cast<T>(rhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (const T &rhs, MouseEventCMD lhs)
{
    return !(rhs != lhs);
}
#endif //MOUSE_COMMANDS_H
