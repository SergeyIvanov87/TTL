#ifndef DESERIALIZE_FRAMEWORK_SPECIALIZATION_H
#define DESERIALIZE_FRAMEWORK_SPECIALIZATION_H

//String specalization
template <>
inline size_t deserializeUnit(std::istream &in, std::string &unit)
{
    in >> unit;
    size_t bytes = unit.size();
    //TODO represent empty string in another way
    if(!unit.compare(emptySerializedValue))
    {
        unit.clear();
    }
    return bytes;
}

//specialization for vector
template <class T>
inline std::enable_if_t<std::is_trivial_v<T>, size_t> deserializeUnit(std::istream &in, Vector<T> &cont)
{
    size_t size = 0;
    in >> size;
    char cLineEndl = '0';
    in.get(cLineEndl);
    size_t elem_count = (size * sizeof(char)) / sizeof(T);
    assert(!((size * sizeof(char)) % sizeof(T)) && "unexpected bytes count in deserialization vector data");
    cont.resize(elem_count);
    in.read(reinterpret_cast<char *>(cont.data()), size);
    return size + sizeof(size);
}

template <class T>
inline std::enable_if_t<! std::is_trivial_v<T>, size_t> deserializeUnit(std::istream &in, Vector<T> &cont)
{
    size_t elem_count = 0;
    in >> elem_count;
    cont.reserve(elem_count);
    size_t bytes = 0;
    for (size_t i = 0; i < elem_count; i++)
    {
        T v{};
        bytes += deserializeUnit<T>(in, v);
        cont.push_back(std::move(v));
    }
    return bytes + sizeof(elem_count);
}

//specialization for std::array
template <class T, size_t N, class U = std::enable_if_t<std::is_trivial_v<T>, size_t>>
inline U deserializeUnit(std::istream &in, std::array<T, N> &cont)
{
    size_t size = 0;
    in >> size;
    assert(size == N * sizeof(T));

    char cLineEndl = '0';
    in.get(cLineEndl);

    in.read(reinterpret_cast<char *>(cont.data()), N * sizeof(T));
    return size + sizeof(size) + sizeof(char);
}

#endif //DESERIALIZE_FRAMEWORK_SPECIALIZATION_H
