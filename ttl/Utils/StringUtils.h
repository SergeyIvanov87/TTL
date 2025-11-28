#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include <string>
#include <list>
#include <numeric>
#include <algorithm>
#include <array>
#include <sstream>
//Wide to_string() implementation, use SFINAE:
//because std::to_string() doesn't have overloaded version for std::string, char *, const char * and etc
//we do not need try make this conversion
//Convert to_string() convertible types only
//Only one function from two version will be instantiated for type T
struct GenericToString
{
    //try to instantiate this method implementation - return std::string without conversion
    //If, std::string can be trivially constructed from T only
    template <class T>
    static typename std::enable_if<
                                std::is_constructible<std::string, const T&>::value,
                                std::string>::type
            toString(T &&value)
    {
        return std::string(std::forward<T>(value));
    }

    //try to instantiate this method implementation - Convert to std::string
    //If, std::string cannot be trivially constructed from T only and type is convertible
    template <class T>
    static typename std::enable_if<
                                ! std::is_constructible<std::string, const T&>::value,
                                std::string>::type
            toString(const T &value)
    {
        return std::to_string(value);
    }

    template <class T, class Modifier>
    static std::string
            toString(std::tuple<Modifier, T> value)
    {
        std::stringstream ss;
        ss << std::get<0>(value) << std::get<1>(value);
        return ss.str();
    }
};


/*
 * You can write your overloaded version std::to_string(ptime, MyClass, and etc ...) here
 * and use makeString("log message, happened at:", ptime, "from :", MyClass) from the box
 */


// Easy to use string builder function that does not require format specification:
template <class ...Arguments>
inline std::string makeString(Arguments &&...args)
{
    static_assert(sizeof...(Arguments) != 0, "makeString arguments are empty");


    std::array<std::string, sizeof...(Arguments)> totalList{GenericToString::toString(std::forward<Arguments>(args))...};
    //calculate total size
    size_t totalLength = std::accumulate(totalList.begin(), totalList.end(), 1, [](const size_t sum, const std::string &str)
    {
        return sum + str.size();
    });
    //use first element as initial accumulator
    std::string &result = *totalList.begin();
    result.reserve(totalLength);
    //accumulate
    std::for_each(std::next(totalList.begin()), totalList.end(), [&result](const std::string &str)
    {
        result += str;
    });
    return result;
}


#endif //STRING_UTILS_H
