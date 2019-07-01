#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H
#include <ostream>
#include <istream>
#include "../SerializeUtilsCommon.hpp"

template<class Impl>
struct ISerializable
{
    //Test child properties
    static constexpr bool isSerializable()
    {
        return isSerializableSelector<Impl>();
    }

    //Public Methods
    template <class ...Params>
    bool serialize(std::ostream &out, Params &&...args)
    {
        if(!m_wasSerialized)
        {
            m_wasSerialized = serializeSelector<Impl>(out, std::forward<Params>(args)...);
        }
        return m_wasSerialized;
    }

    template <class ...Params>
    bool deserialize(std::istream &in, Params &&...args)
    {
        bool ret = deserializeSelector<Impl>(in, std::forward<Params>(args)...);
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
        return getObjectSizeSelector<Impl>();
    }

    //-----------------SFINAE---------------------------------------------------
    //1) Is SerializableSelector
    template<class Derived>
    static constexpr bool isSerializableSelector()
    {
        return Derived::isSerializableSupport;
    }

    //2) serializeSelector
    template <class Derived, class ...Params>
    typename std::enable_if<Derived::isSerializableSupport, bool>::type
    serializeSelector(std::ostream &out, Params &&...args)
    {
        return static_cast<Impl *>(this)->serializeImpl(out, std::forward<Params>(args)...);
    }

    template <class Derived, class ...Params>
    typename std::enable_if< !Derived::isSerializableSupport, bool>::type
    serializeSelector(std::ostream &out, Params &&...args)
    {
        return true;
    }

    //3) deserializeSelector
    template <class Derived, class ...Params>
    typename std::enable_if<Derived::isSerializableSupport, bool>::type
    deserializeSelector(std::istream &in, Params &&...args)
    {
        return static_cast<Impl *>(this)->deserializeImpl(in, std::forward<Params>(args)...);
    }

    template <class Derived, class ...Params>
    typename std::enable_if< !Derived::isSerializableSupport, bool>::type
    deserializeSelector(std::istream &in, Params &&...args)
    {
        return true;
    }

    //4) getObjectSizeSelector
    template <class Derived>
    typename std::enable_if<Derived::isDumpObjectSupport, size_t>::type
    getObjectSizeSelector() const
    {
        return static_cast<const Impl *>(this)->getObjectSizeImpl();
    }

    template <class Derived>
    typename std::enable_if< !Derived::isDumpObjectSupport, size_t>::type
    getObjectSizeSelector() const
    {
        return 0;
    }
private:
    bool m_wasSerialized = false;
};
#endif //ISERIALIZABLE_H
