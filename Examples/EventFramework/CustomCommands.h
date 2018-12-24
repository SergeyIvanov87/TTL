#ifndef TEST_CUSTOM_EVENT_COMMANDS_H
#define TEST_CUSTOM_EVENT_COMMANDS_H

enum class CustomEventCMD
{
    TEID_1_CMD,
    MAX__EVENT_TYPE
};

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (const T &lhs, CustomEventCMD rhs)
{
    return lhs != static_cast<T>(rhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (CustomEventCMD lhs, const T &rhs)
{
    return !(rhs != lhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (CustomEventCMD rhs, const T &lhs)
{
    return lhs != static_cast<T>(rhs);
}

template<class T>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (const T &rhs, CustomEventCMD lhs)
{
    return !(rhs != lhs);
}

#endif //TEST_CUSTOM_EVENT_COMMANDS_H
