#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H
#include <ostream>
#include <istream>
#include "../SerializeUtilsCommon.hpp"

template<class Impl>
struct ISerializable
{
    struct ___W {
        using D = Impl;
    };
    friend class ___W::D;

    //Public Methods
    static constexpr bool isSerializable()
    {
        return is_serializable_request<Impl>();
    }

    template <class ...Params>
    size_t serialize(std::ostream &out, Params &&...args) const
    {
        return serialize_request<Impl>(out, std::forward<Params>(args)...);
    }

    template <class ...Params>
    size_t deserialize(std::istream &in, Params &&...args)
    {
        return deserialize_request<Impl>(in, std::forward<Params>(args)...);
    }

    size_t getObjectSize() const
    {
        return get_object_size_request<Impl>();
    }

private:
    template<class Derived>
    static constexpr bool is_serializable_request()
    {
        std::stringstream test;
        return not std::is_same_v<decltype(std::declval<Derived>().onSerialize(test)), int>;
    }

    template <class Derived, class ...Params>
    size_t serialize_request(std::ostream &out, Params &&...args) const
    {
        return static_cast<const Impl *>(this)->onSerialize(out, std::forward<Params>(args)...);
    }

    template <class ...Params>
    constexpr int onSerialize(std::ostream &, Params &&...) const noexcept
    {
        return 0;
    }

    template <class Derived, class ...Params>
    size_t deserialize_request(std::istream &in, Params &&...args)
    {
        return static_cast<Impl *>(this)->onDeserialize(in, std::forward<Params>(args)...);
    }

    template <class ...Params>
    constexpr int onDeserialize(std::istream &, Params &&...) noexcept
    {
        return 0;
    }

    template <class Derived>
    size_t get_object_size_request() const
    {
        return static_cast<const Impl *>(this)->onGetObjectSize();
    }

    constexpr size_t onGetObjectSize() const noexcept
    {
        return 0;
    }
};










template<class Impl>
struct ISerializableIntrusive
{
    //Test child properties
    static constexpr bool isSerializable()
    {
        return is_serializable_request<Impl>();
    }

    //Public Methods
    template <class ...Params>
    bool serialize(std::ostream &out, Params &&...args)
    {
        if(!m_wasSerialized)
        {
            m_wasSerialized = serialize_request<Impl>(out, std::forward<Params>(args)...);
        }
        return m_wasSerialized;
    }

    template <class ...Params>
    bool deserialize(std::istream &in, Params &&...args)
    {
        bool ret = deserialize_request<Impl>(in, std::forward<Params>(args)...);
        if(ret)
        {
            m_wasSerialized = false;
        }
        return ret;
    }

    bool wasSerialized() const
    {
        return m_wasSerialized;
    }

    size_t getObjectSize() const
    {
        return get_object_size_request<Impl>();
    }

    //-----------------SFINAE---------------------------------------------------
    //1) Is SerializableSelector
    template<class Derived>
    static constexpr bool is_serializable_request()
    {
        return Derived::isSerializableSupport;
    }

    //2) serializeSelector
    template <class Derived, class ...Params>
    typename std::enable_if<Derived::isSerializableSupport, bool>::type
    serialize_request(std::ostream &out, Params &&...args)
    {
        return static_cast<Impl *>(this)->onSerialize(out, std::forward<Params>(args)...);
    }

    template <class Derived, class ...Params>
    typename std::enable_if< !Derived::isSerializableSupport, bool>::type
    serialize_request(std::ostream &out, Params &&...args)
    {
        return true;
    }

    //3) deserialize_request
    template <class Derived, class ...Params>
    typename std::enable_if<Derived::isSerializableSupport, bool>::type
    deserialize_request(std::istream &in, Params &&...args)
    {
        return static_cast<Impl *>(this)->onDeserialize(in, std::forward<Params>(args)...);
    }

    template <class Derived, class ...Params>
    typename std::enable_if< !Derived::isSerializableSupport, bool>::type
    deserialize_request(std::istream &in, Params &&...args)
    {
        return true;
    }

    //4) get_object_size_request
    template <class Derived>
    typename std::enable_if<Derived::isDumpObjectSupport, size_t>::type
    get_object_size_request() const
    {
        return static_cast<const Impl *>(this)->getObjectSizeImpl();
    }

    template <class Derived>
    typename std::enable_if< !Derived::isDumpObjectSupport, size_t>::type
    get_object_size_request() const
    {
        return 0;
    }
private:
    bool m_wasSerialized = false;
};
#endif //ISERIALIZABLE_H
