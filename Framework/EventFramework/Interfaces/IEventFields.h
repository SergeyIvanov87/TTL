#ifndef IEVENT_FIELDS
#define IEVENT_FIELDS

template<class Impl>
struct IEventField
{
    static Impl createFromString(const std::string &str)
    {
        return Impl::createFromStringImpl(str);
    }

    const char *toCString() const noexcept
    {
        return self()->toCStringImpl();
    }

    std::pair<bool, std::string> validate() const
    {
        return self()->validateImpl();
    }

    auto value() const noexcept
    {
        return self()->valueImpl();
    }

private:
    Impl *self() noexcept
    {
        return static_cast<Impl*>(this);
    }
    const Impl *self() const noexcept
    {
        return static_cast<const Impl*>(this);
    }

    std::pair<bool, std::string> validateImpl() const
    {
        return  {true, ""};
    }

};

//Comparison operators
template<class T, template <class> class IEventField, class Impl>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator== (const T &lhs, const IEventField<Impl> &rhs)
{
    return lhs == lhs.value();
}
template<template <class> class IEventField, class Impl>
constexpr bool
    operator== (const IEventField<Impl> &lhs, const IEventField<Impl> &rhs)
{
    return lhs.value() == rhs.value();
}


template<class T, template <class> class IEventField, class Impl>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator!= (const T &lhs, const IEventField<Impl> &rhs)
{
    return !(lhs == lhs.value());
}
template<template <class> class IEventField, class Impl>
constexpr bool
    operator!= (const IEventField<Impl> &lhs, const IEventField<Impl> &rhs)
{
    return !(lhs.value() == rhs.value());
}


template<class T, template <class> class IEventField, class Impl>
constexpr typename std::enable_if<std::is_integral<T>::value, bool>::type
    operator< (const T &lhs, const IEventField<Impl> &rhs)
{
    return lhs < lhs.value();
}

template<template <class> class IEventField, class Impl>
constexpr bool
    operator< (const IEventField<Impl> &lhs, const IEventField<Impl> &rhs)
{
    return lhs.value() < rhs.value();
}


template <class Impl>
struct IEventTriggerCommand
{
    static Impl createFromString(const std::string &commandStr)
    {
        return Impl::createFromStringImpl(commandStr);
    }

    const char *toCString() const noexcept
    {
        return self()->toCStringImpl();
    }

    std::pair<bool, std::string> validate() const
    {
        return self()->validateImpl();
    }

    auto value() const noexcept
    {
        return self()->valueImpl();
    }

private:
    Impl *self() noexcept
    {
        return static_cast<Impl*>(this);
    }
    const Impl *self() const noexcept
    {
        return static_cast<const Impl*>(this);
    }
};
#endif
