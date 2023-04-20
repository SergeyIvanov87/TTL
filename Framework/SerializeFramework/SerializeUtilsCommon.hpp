#ifndef SERIALIZE_UTILS_HPP
#define SERIALIZE_UTILS_HPP

#include <numeric>
#include <ostream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <cassert>

template <class T>
class ISerializable;

template <class T>
class ISerializableIntrusive;


//Serialization
static constexpr const char* emptySerializedValue = "--empty--";
static constexpr size_t emptySerializedValueSize = 10;

using Vector2BytesResult = std::pair<const char *, size_t>;
template <class Vector>
inline Vector2BytesResult vector2Bytes(const Vector &v)
{
    if(v.empty())
    {
        return std::make_pair(nullptr, 0);
    }
    return std::make_pair(reinterpret_cast<const char*>(v.data()),
            v.size() * sizeof(v[0]) / sizeof(char));
}

//Common serialize for base types
template <class T>
inline std::enable_if_t<not std::is_base_of_v<ISerializableIntrusive<T>, T> &&
                        not std::is_base_of_v<ISerializable<T>, T>, size_t> serializeUnit(std::ostream &out, const T &unit)
{
    out << unit << std::endl;
    return sizeof(std::decay_t<T>);
}


template <class T>
inline std::enable_if_t<std::is_base_of_v<ISerializableIntrusive<T>, T>, size_t> serializeUnit(std::ostream &out, const T &unit)
{
    return const_cast<T&>(unit).serialize(out);
}

template <class T>
inline std::enable_if_t<std::is_base_of_v<ISerializable<T>, T>, size_t> serializeUnit(std::ostream &out, const T &unit)
{
    return unit.serialize(out);
}


#include "FrameworkSpecializations/SerializeFrameworkSpecialization/SerializeSpecificTypes.h"

//Serialize variadic params
template<class ...Params>
size_t serializeParams(std::ostream &out, Params &&...params)
{
    size_t expander[]
    {
        0, serializeUnit(out, params)...
    };
    return std::accumulate(std::begin(expander), std::end(expander), 0);
}


//Deserialization
//Common for base types
template <class T>
inline std::enable_if_t<not std::is_base_of_v<ISerializableIntrusive<T>, T> &&
                        not std::is_base_of_v<ISerializable<T>, T>, size_t> deserializeUnit(std::istream &in, T &unit)
{
    in >> unit;
    return sizeof(T);
}

template <class T>
inline std::enable_if_t<std::is_base_of_v<ISerializableIntrusive<T>, T>, size_t> deserializeUnit(std::istream &in, T &unit)
{
    return unit.deserialize(in);
}

template <class T>
inline std::enable_if_t<std::is_base_of_v<ISerializable<T>, T>, size_t> deserializeUnit(std::istream &in, T &unit)
{
    return unit.deserialize(in);
}

#include "FrameworkSpecializations/SerializeFrameworkSpecialization/DeSerializeSpecificTypes.h"

//Deserialize variadic params
template<class ...Params>
size_t deserializeParams(std::istream &in, Params &...params)
{
    size_t expander[] {0, deserializeUnit(in, params)...};
    return std::accumulate(std::begin(expander), std::end(expander), 0);
}
#endif //SERIALIZE_UTILS_HPP
