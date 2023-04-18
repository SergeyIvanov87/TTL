#ifndef SERIALIZE_FRAMEWORK_SPECIALIZATION_H
#define SERIALIZE_FRAMEWORK_SPECIALIZATION_H

template <>
inline size_t serializeUnit(std::ostream &out, const std::string &unit)
{
    out << (unit.empty() ? emptySerializedValue : unit) << std::endl;
    return (unit.empty() ? emptySerializedValueSize : unit.size());
}

//specialization for vector
template<typename T> using Vector = std::vector<T>;

template <class T>
inline size_t serializeUnit(std::ostream &out, const Vector<T> &cont)
{
    auto result = vector2Bytes(cont);
    out << result.second << std::endl; //in bytes
    out.write(result.first, result.second);
    out << std::endl;
    return result.second + sizeof(result.second);
}

//specialization for std::array
template <class T, size_t N>
inline size_t serializeUnit(std::ostream &out, const std::array<T, N> &cont)
{
    auto result = vector2Bytes(cont);
    const auto BYTES = N * sizeof(T);
    out << BYTES << std::endl; //in bytes
    out.write(result.first, result.second);
    out << std::endl;
    return BYTES * sizeof(BYTES);
}

//specialization for std::set
template<typename T> using Set = std::set<T>;
//TODO

#endif //SERIALIZE_FRAMEWORK_SPECIALIZATION_H
