#ifndef DESERIALIZE_FRAMEWORK_SPECIALIZATION_H
#define DESERIALIZE_FRAMEWORK_SPECIALIZATION_H

//String specalization
template <>
inline bool deserializeUnit(std::istream &in, std::string &unit)
{
    in >> unit;
    if(!unit.compare(emptySerializedValue))
    {
        unit.clear();
    }
    return true;
}

//specialization for vector
template <class T>
inline bool deserializeUnit(std::istream &in, Vector<T> &cont)
{
    size_t size = 0;
    in >> size;
    char cLineEndl = '0';
    in.get(cLineEndl);
    cont.resize( (size * sizeof(char)) / sizeof(T) );
    in.read(reinterpret_cast<char *>(cont.data()), size);
    return true;
}


//specialization for std::array
template <class T, size_t N>
inline bool deserializeUnit(std::istream &in, std::array<T, N> &cont)
{
    size_t size = 0;
    in >> size;
    assert(size == N * sizeof(T));

    char cLineEndl = '0';
    in.get(cLineEndl);

    in.read(reinterpret_cast<char *>(cont.data()), N * sizeof(T));
    return true;
}

#endif //DESERIALIZE_FRAMEWORK_SPECIALIZATION_H
