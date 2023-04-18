#ifndef SERIALIZE_UTILS_H
#define SERIALIZE_UTILS_H

//Serialization
template <class T>
inline size_t serializeUnit(std::ostream &out, const T &unit);

template<class ...Params>
size_t serializeParams(std::ostream &out, Params ...params);

//Deserialization
template <class T>
inline bool deserializeUnit(std::istream &in, T &unit);

template<class ...Params>
void deserializeParams(std::istream &in, Params &...params);
#endif //SERIALIZE_UTILS_HPP
