#ifndef SERIALIZE_UTILS_HPP
#define SERIALIZE_UTILS_HPP
#include <ostream>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <cassert>

//Serialization
static constexpr const char* emptySerializedValue = "--empty--";

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
inline bool serializeUnit(std::ostream &out, const T &unit)
{
    out << unit << std::endl;
    return true;
}

#include "FrameworkSpecializations/SerializeFrameworkSpecialization/SerializeSpecificTypes.h"

//Serialize variadic params
template<class ...Params>
void serializeParams(std::ostream &out, Params ...params)
{
    using expander = int[];
    (void)expander {0, serializeUnit(out, params)...};
}


//Deserialization
//Common for base types
template <class T>
inline bool deserializeUnit(std::istream &in, T &unit)
{
    in >> unit;
    return true;
}

#include "FrameworkSpecializations/SerializeFrameworkSpecialization/DeSerializeSpecificTypes.h"

//Deserialize variadic params
template<class ...Params>
void deserializeParams(std::istream &in, Params &...params)
{
    using expander = int[];
    (void)expander {0, deserializeUnit(in, params)...};
}
#endif //SERIALIZE_UTILS_HPP
