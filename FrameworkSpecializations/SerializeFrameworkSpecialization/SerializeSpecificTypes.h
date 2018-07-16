#ifndef SERIALIZE_FRAMEWORK_SPECIALIZATION_H
#define SERIALIZE_FRAMEWORK_SPECIALIZATION_H

template <>
inline bool serializeUnit(std::ostream &out, const std::string &unit)
{
    out << (unit.empty() ? emptySerializedValue : unit) << std::endl;
    return true;
}

//specialization for vector
template<typename T> using Vector = std::vector<T>;

template <class T>
inline bool serializeUnit(std::ostream &out, const Vector<T> &cont)
{
    auto result = vector2Bytes(cont);
    out << result.second << std::endl; //in bytes
    out.write(result.first, result.second);
    out << std::endl;
    return true;
}

//specialization for std::array
template <class T, size_t N>
inline bool serializeUnit(std::ostream &out, const std::array<T, N> &cont)
{
    auto result = vector2Bytes(cont);
    out << N * sizeof(T) << std::endl; //in bytes
    out.write(result.first, result.second);
    out << std::endl;
    return true;
}

//specialization for std::set
template<typename T> using Set = std::set<T>;
//TODO

#endif //SERIALIZE_FRAMEWORK_SPECIALIZATION_H
