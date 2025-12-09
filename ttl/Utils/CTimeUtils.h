#ifndef CTIMEUTILS_H
#define CTIMEUTILS_H

#include <cstddef>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <type_traits>
#include <tuple>

namespace CTimeUtils
{

//Index for element T in tuple
template <class T, class Tuple>
struct Index;

template <class T, class... Types>
struct Index<T, std::tuple<T, Types...>> {
    static const std::size_t value = 0;
};

template <class T, class U, class... Types>
struct Index<T, std::tuple<U, Types...>> {
    static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
};


/*******************Enums to int*************************************************************/
template<typename E>
constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
{
   return static_cast<typename std::underlying_type<E>::type>(e);
}
/***************************TUPLE************************************************/
//------------------Base class for tuple traits

/*Tuple binary data is in reverse-order. It means
 * tuple<A,B,C,D,E> presents in memory as
 * E-D-C-B-A
 * TupleElementsTraits class represents offset_in_bytes in this reverse-order
 * So class A has a max byte_offset
 * class E has a 0 offset
 */
template<std::size_t Index, class Tuple>
struct TupleElementsTraits
{
    using type = typename std::tuple_element<Index, Tuple>::type;
    enum
    {
        index = Index,
        size_of_type = sizeof(type),
        bytes_offset = TupleElementsTraits<Index -1, Tuple>::bytes_offset - size_of_type/*TupleElementsTraits<Index -1, Tuple>::size_of_type +*/
    };
};

template<class Tuple>
struct TupleElementsTraits<0, Tuple>
{
    using type = typename std::tuple_element<0, Tuple>::type;
    enum
    {
        index = 0,
        size_of_type = sizeof(type),
        bytes_offset = sizeof(Tuple) - size_of_type
    };
};

//FOR_EACH
//1) get elements from tuple Object
template<class Tuple, class F, std::size_t ...Is>
constexpr void for_each_in_tuple_impl(Tuple &tuple, F func, std::index_sequence<Is...> seq)
{
    using expander = int[];
    (void)expander { 0, ((void)func((size_t)Is, std::get<Is>(tuple)), 0)... };
}

template<class Tuple, class F>
constexpr void for_each_in_tuple(Tuple &tuple, F func)
{
    for_each_in_tuple_impl(tuple, func, std::make_index_sequence< std::tuple_size<Tuple>::value>{});
}
/*
template<class Tuple, class F, std::size_t ...Is>
constexpr void for_each_in_tuple_traits_impl(Tuple &tuple, F func, std::index_sequence<Is...> seq)
{
    //code-monkey... Use intsance of TupleElementsTraits in functor to determite traits type((
    using expander = int[];
    (void)expander { 0, ((void)func(TupleElementsTraits<Is, Tuple>(), //std::declvar<TupleElementsTraits>
        std::get<Is>(tuple)), 0)... };
}*/

template<class Tuple, class F, std::size_t ...Is>
constexpr void for_each_in_tuple_traits_impl(Tuple &tuple, F func, std::index_sequence<Is...> seq)
{
    //code-monkey... Use intsance of TupleElementsTraits in functor to determite traits type((
    using expander = int[];
    (void)expander { 0, ((void)func(TupleElementsTraits<Is, Tuple>(), //std::declvar<TupleElementsTraits>
        std::get<Is>(tuple)), 0)... };
}

template<class Tuple, class F>
constexpr void for_each_in_tuple_traits(Tuple &tuple, F func)
{
    for_each_in_tuple_traits_impl(tuple, func, std::make_index_sequence< std::tuple_size<Tuple>::value>{});
}

template<class Tuple, class F, std::size_t ...Is>
constexpr void for_each_type_in_tuple_traits_impl( F func, std::index_sequence<Is...> seq)
{
    //code-monkey... Use intsance of TupleElementsTraits in functor to determite traits type((
    using expander = int[];
    (void)expander { 0, ((void)func(TupleElementsTraits<Is, Tuple>()), 0)... };
}

template<class Tuple, class F>
constexpr void for_each_type_in_tuple_traits(F func)
{
    for_each_type_in_tuple_traits_impl<Tuple>( func, std::make_index_sequence< std::tuple_size<Tuple>::value>{});
}

/*******************************TUPLE TRAITS CUSTOM TYPE************************/


template<class Tuple, class F>
constexpr void for_each_in_tuple_user_traits(Tuple &tuple, F func)
{
    for_each_in_tuple_traits_impl(tuple, func, std::make_index_sequence< std::tuple_size<Tuple>::value>{});
}
/*******************************************************/

//2) for static Tuple Type
template<class F, std::size_t ...Is>
constexpr void for_each_index_in_tuple_impl(F func, std::index_sequence<Is...> seq)
{
    using expander = int[];
    (void)expander { 0, ((void)func((size_t)Is), 0)... };
}

template<class Tuple, class F>
constexpr void for_each_index_in_tuple(F func)
{
    for_each_index_in_tuple_impl(func, std::make_index_sequence< std::tuple_size<Tuple>::value>{});
}

/****************************INT 2 Literal utils***********************************************/
/* IMPLEMENTATION */
/* calculate absolute value */
constexpr int abs_val (int x)
    { return x < 0 ? -x : x; }

/* calculate number of digits needed, including minus sign */
constexpr int num_digits (int x)
    { return x < 0 ? 1 + num_digits (-x) : x < 10 ? 1 : 1 + num_digits (x / 10); }

/* metaprogramming string type: each different string is a unique type */
template<char... args>
struct metastring {
   /*static constexpr */const char data[sizeof... (args) + 9] = {'B', 'o', 'n', 'e', 'I', 'n', 'f', 'o', '_', args...};
//    enum {SI = sizeof... (args)};
//    const std::array<char, SI> data{args...};
};

/* recursive number-printing template, general case (for three or more digits) */
template<int size, int x, char... args>
struct numeric_builder {
    typedef typename numeric_builder<size - 1, x / 10, '0' + abs_val (x) % 10, args...>::type type;
};

/* special case for two digits; minus sign is handled here */
template<int x, char... args>
struct numeric_builder<2, x, args...> {
    typedef metastring<x < 0 ? '-' : '0' + x / 10, '0' + abs_val (x) % 10, args...> type;
};

/* special case for one digit (positive numbers only) */
template<int x, char... args>
struct numeric_builder<1, x, args...> {
    typedef metastring<'0' + x, args...> type;
};

/* convenience wrapper for numeric_builder */
template<int x>
class numeric_string
{
private:
    /* generate a unique string type representing this number */
    typedef typename numeric_builder<num_digits (x), x, '\0'>::type type;

    /* declare a static string of that type (instantiated later at file scope) */
    static constexpr type value {};

public:
    /* returns a pointer to the instantiated string */
    inline static constexpr const char * get ()
        { return value.data; }

    inline static constexpr const char (&getArray())[num_digits (x) + 1]
        { return value.data; }
};

/* instantiate numeric_string::value as needed for different numbers */
template<int x>
constexpr typename numeric_string<x>::type numeric_string<x>::value;


/*****************Pack char array with digit*************************************************************/
template <char ...args>
struct metadigipackstring
{
     const char data[sizeof... (args)] = {args...};
};

template <int Digit, char ...args>
struct metadigipackstringpacker
{
    enum
    {
        FIRST = (Digit & 0xFF000000) >> 24,
        SECOND = (Digit & 0x00FF0000) >> 16,
        THIRD = (Digit & 0x0000FF00) >> 8,
        FOURTH = Digit & 0x000000FF
    };
    //PACK TO LSB, unpack it later and convert to MSB if needed
    //typedef metadigipackstring<args..., FIRST, SECOND, THIRD, FOURTH, '\0'> type;
    typedef metadigipackstring<args..., FOURTH, THIRD, SECOND, FIRST, '\0'> type;
};

template <int Digit, char ...args>
struct Packer
{
     typedef typename metadigipackstringpacker<Digit,args...>::type type;

    /* declare a static string of that type (instantiated later at file scope) */
    static constexpr type value {};
    inline static constexpr const char *getPackedData() { return value.data; }
};

template<int Digit, char ...args>
constexpr typename Packer<Digit, args...>::type Packer<Digit, args...>::value;
}
#endif /* CTIMEUTILS_H */
