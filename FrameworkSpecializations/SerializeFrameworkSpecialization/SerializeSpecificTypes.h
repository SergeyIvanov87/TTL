#ifndef SERIALIZE_FRAMEWORK_SPECIALIZATION_H
#define SERIALIZE_FRAMEWORK_SPECIALIZATION_H

//TODO  remove endline / std::endl?
template <>
inline size_t serializeUnit(std::ostream &out, const std::string &unit)
{
    out << (unit.empty() ? emptySerializedValue : unit) << std::endl;
    return (unit.empty() ? emptySerializedValueSize : unit.size());
}

//specialization for vector
template<typename T> using Vector = std::vector<T>;
//// partial specialization SFINAE for trivial types
template <class T>
inline std::enable_if_t<std::is_trivial_v<T>, size_t> serializeUnit(std::ostream &out, const Vector<T> &cont)
{
    auto result = vector2Bytes(cont);
    out << result.second << std::endl; //in bytes
    out.write(result.first, result.second);
    out << std::endl;
    return result.second + sizeof(result.second);
}

//// partial specialization SFINAE for complex types
template <class T>
inline std::enable_if_t<!std::is_trivial_v<T>, size_t> serializeUnit(std::ostream &out, const Vector<T> &cont)
{
    size_t bytes = 0;
    size_t count = cont.size();

    out << count;
    bytes += sizeof(count);
    for(const auto& v: cont)
    {
        bytes += serializeUnit(out, v);
    }
    return bytes;
}

//specialization for std::array
template <class T, size_t N, class U = std::enable_if_t<std::is_trivial_v<T>, size_t>>
inline U serializeUnit(std::ostream &out, const std::array<T, N> &cont)
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
