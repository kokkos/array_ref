///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_324BD9FF_856B_4DC7_BC6F_2A93F3DF63CD)
#define STD_324BD9FF_856B_4DC7_BC6F_2A93F3DF63CD

#include <array>
#include <initializer_list>
#include <tuple>
#include <utility>
#include <type_traits>

#include "detail/fwd.hpp"
#include "detail/meta_logical_operators.hpp"

namespace std
{

///////////////////////////////////////////////////////////////////////////////

template <std::size_t... Dims>
struct rank<experimental::dimensions<Dims...> >
  : integral_constant<std::size_t, sizeof...(Dims)> {};

template <typename... Params>
struct rank<experimental::mdspan<Params...> >
  : integral_constant<
        std::size_t
      , experimental::mdspan<Params...>::rank()
    > {};

template <std::size_t Head, std::size_t... Tail>
struct extent<experimental::dimensions<Head, Tail...>, 0>
  : integral_constant<std::size_t, Head> {};

template <std::size_t Head, std::size_t... Tail, unsigned ND>
struct extent<experimental::dimensions<Head, Tail...>, ND>
  : integral_constant<
        std::size_t
      , extent<experimental::dimensions<Tail...>, ND - 1>::value
    > {};

template <typename... Params, unsigned ND>
struct extent<experimental::mdspan<Params...>, ND>
  : extent<typename experimental::mdspan<Params...>::dimensions, ND> {};

namespace experimental { namespace detail
{

///////////////////////////////////////////////////////////////////////////////

template <typename... Ts>
struct type_list
{
    using type = type_list;

    static constexpr std::size_t size() noexcept
    {
        return sizeof...(Ts);
    }
};

///////////////////////////////////////////////////////////////////////////////

template <typename T, T... I>
struct integer_sequence_array<integer_sequence<T, I...> >
{
    using size_type = std::size_t;
    using value_type = T;

    static constexpr value_type value[] = { I... };

    static constexpr size_type size() noexcept
    {
        return sizeof...(I);
    }

    constexpr value_type operator[] (size_type idx) const noexcept
    {
        return value[idx];
    }
};

template <typename T, T... I>
constexpr T integer_sequence_array<integer_sequence<T, I...> >::value[];

///////////////////////////////////////////////////////////////////////////////

template <typename Key, typename Value, Key K, Value V>
struct integral_pair
{
    using type = integral_pair;

    using key_type = Key;
    static constexpr key_type key = K;

    using value_type = Value;
    static constexpr value_type value = V;
}; 

///////////////////////////////////////////////////////////////////////////////

struct type_key_less
{
    template <typename T0, typename T1>
    struct apply
      : integral_constant<bool, T0::key < T1::key> {};
};

///////////////////////////////////////////////////////////////////////////////

struct type_value_less
{
    template <typename T0, typename T1>
    struct apply 
      : integral_constant<bool, T0::value < T1::value> {};
};

///////////////////////////////////////////////////////////////////////////////

template <typename T, typename... Tail> 
struct type_list_push_front<T, type_list<Tail...> > : type_list<T, Tail...> {};

///////////////////////////////////////////////////////////////////////////////

template <typename T, typename... Tail, typename Compare>
struct type_list_push<T, type_list<Tail...>, Compare>
  : type_list_push_impl<Compare, T, Tail...> {};

template <typename Compare, typename T>
struct type_list_push_impl<Compare, T> : type_list<T> {};

template <typename Compare, typename T0, typename T1, typename... Tail> 
struct type_list_push_impl<Compare, T0, T1, Tail...>
  : type_list_push_front<
        typename conditional<
            Compare::template apply<T0, T1>::value, T0, T1
        >::type
      , typename type_list_push_impl<
            Compare
          , typename conditional<
                Compare::template apply<T1, T0>::value, T0, T1
            >::type
          , Tail...
        >::type
    > {};

///////////////////////////////////////////////////////////////////////////////

template <typename Compare> 
struct type_list_sort<type_list<>, Compare> : type_list<> {};

template <typename T, typename... Tail, typename Compare>
struct type_list_sort<type_list<T, Tail...>, Compare>
  : type_list_push<
        T
      , typename type_list_sort<type_list<Tail...>, Compare>::type
    > {};

///////////////////////////////////////////////////////////////////////////////

template <typename Key, Key... Ks, typename Value, Value... Vs>
struct make_key_value_type_list<
           integer_sequence<Key, Ks...>
         , integer_sequence<Value, Vs...>
       >
{
    using type = type_list<integral_pair<Key, Value, Ks, Vs>...>;
};

///////////////////////////////////////////////////////////////////////////////

template <typename T, T... I>
struct make_key_value_type_list_from_integer_sequence<integer_sequence<T, I...> >
  : make_key_value_type_list<
        typename make_integer_sequence<std::size_t, sizeof...(I)>::type
      , integer_sequence<T, I...>
    > {};

///////////////////////////////////////////////////////////////////////////////

template <typename T, typename... KeyValues>
struct make_integer_sequence_from_keys<T, type_list<KeyValues...> >
  : integer_sequence<T, KeyValues::key...> {};

///////////////////////////////////////////////////////////////////////////////

// 1.) Create a sequence of key-value pairs from the input sequence. O(n)
// 2.) Sort the new key-value sequence by key. O(sort(n))
// 3.) Create a new integer_sequence of sorted keys from the key-value
//     sequence. O(n)

template <typename T, T... I>
struct make_integer_sequence_inverse_mapping<integer_sequence<T, I...> >
  : make_integer_sequence_from_keys<                                 // 3.)
        std::size_t
      , typename type_list_sort<                                     // 2.)
            typename make_key_value_type_list_from_integer_sequence< // 1.)
                integer_sequence<T, I...>
            >::type
        >::type
    > {};

///////////////////////////////////////////////////////////////////////////////

template <std::size_t N, typename Dimensions>
struct rank_is_greater_than
{
    static constexpr bool value = N < Dimensions::rank();
};

template <std::size_t N, typename Dimensions>
struct rank_is_equal_to
{
    static constexpr bool value = N == Dimensions::rank();
};

template <std::size_t N, typename Dimensions, typename T>
struct rank_is_unit_stride<N, Dimensions, integer_sequence<T> >
{
    static constexpr bool value = true;
};

template <std::size_t N, typename Dimensions, typename T, T I0, T... Is>
struct rank_is_unit_stride<N, Dimensions, integer_sequence<T, I0, Is...> >
{
    static constexpr bool value = N == I0;
};

template <std::size_t N, typename Dimensions>
struct rank_is_last_index
{
    static constexpr bool value =    (1 < Dimensions::rank()) 
                                  && (N == (Dimensions::rank() - 1));
};

///////////////////////////////////////////////////////////////////////////////

// Base case.
template <>
struct count_dynamic_dims<> : integral_constant<size_t, 0> {};

template <std::size_t Head, std::size_t... Tail>
struct count_dynamic_dims<Head, Tail...>
  : integral_constant<std::size_t,
        ( Head == dyn
        ? count_dynamic_dims<Tail...>::value + 1
        : count_dynamic_dims<Tail...>::value) 
    > {};

///////////////////////////////////////////////////////////////////////////////

template <std::size_t... Dims>
struct make_dynamic_dims_array
{
    using type = array<std::size_t, count_dynamic_dims<Dims...>::value>;
};

///////////////////////////////////////////////////////////////////////////////

template <std::size_t Value, std::size_t... Dims>
struct make_filled_dims<0, Value, Dims...>
{
    using type = dimensions<Dims...>;
};

template <std::size_t N, std::size_t Value, std::size_t... Dims>
struct make_filled_dims : make_filled_dims<N - 1, Value, Dims..., Value> {};

///////////////////////////////////////////////////////////////////////////////

// Base case.
template <>
struct pack_is_integral<> : true_type {};

template <typename Head, typename... Tail>
struct pack_is_integral<Head, Tail...>
  : experimental::conjunction<is_integral<Head>, pack_is_integral<Tail...> > {};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_integral_range_slice_specifier : false_type {};

// initializer_list<I0> case
template <typename I0>
struct is_integral_range_slice_specifier<initializer_list<I0> >
  : is_integral<typename decay<I0>::type> {};

// pair<I0, I1> case
template <typename I0, typename I1>
struct is_integral_range_slice_specifier<pair<I0, I1> >
  : experimental::conjunction<
        is_integral<typename decay<I0>::type>
      , is_integral<typename decay<I1>::type>
    > {};

// tuple<I0, I1> case
template <typename I0, typename I1>
struct is_integral_range_slice_specifier<tuple<I0, I1> >
  : experimental::conjunction<
        is_integral<typename decay<I0>::type>
      , is_integral<typename decay<I1>::type>
    > {};

// array<I0, 2> case
template <typename I0>
struct is_integral_range_slice_specifier<array<I0, 2> >
  : is_integral<typename decay<I0>::type> {};

template <>
struct is_integral_range_slice_specifier<all_tag>
  : true_type {};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_slice_specifier
  : experimental::disjunction<
        is_integral_range_slice_specifier<T>
      , is_integral<typename decay<T>::type>
    > {};

///////////////////////////////////////////////////////////////////////////////

// Base case.
template <>
struct pack_is_slice_specifiers<> : true_type {};

template <typename Head, typename... Tail>
struct pack_is_slice_specifiers<Head, Tail...>
  : experimental::conjunction<
        is_slice_specifier<Head>
      , pack_is_slice_specifiers<Tail...>
    > {};

///////////////////////////////////////////////////////////////////////////////

// Base case.
template <>
struct count_integral_range_slice_specifiers<>
  : integral_constant<std::size_t, 0> {};

template <typename Head, typename... Tail>
struct count_integral_range_slice_specifiers<Head, Tail...>
  : integral_constant<std::size_t,
        ( is_integral_range_slice_specifier<Head>::value
        ? count_integral_range_slice_specifiers<Tail...>::value + 1
        : count_integral_range_slice_specifiers<Tail...>::value) 
    > {};

} // std::experimental::detail

template <typename T, std::size_t... Dims>
struct extract_dimensions
{
    using type = dimensions<Dims...>;    
};

template <typename T, std::size_t... Dims>
struct extract_dimensions<T[], Dims...>
{
    using type = typename extract_dimensions<T, Dims..., dyn>::type;
};

template <typename T, std::size_t N, std::size_t... Dims>
struct extract_dimensions<T[N], Dims...>
{
    using type = typename extract_dimensions<T, Dims..., N>::type;
};

}} // std::experimental

#endif // STD_324BD9FF_856B_4DC7_BC6F_2A93F3DF63CD

