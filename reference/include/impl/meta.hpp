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

#include "impl/fwd.hpp"
#include "impl/meta_logical_operators.hpp"

namespace std
{

///////////////////////////////////////////////////////////////////////////////

template <std::size_t... Dims>
struct rank<experimental::dimensions<Dims...> >
  : integral_constant<std::size_t, sizeof...(Dims)> {};

template <typename... Params>
struct rank<experimental::array_ref<Params...> >
  : integral_constant<
        std::size_t
      , experimental::array_ref<Params...>::rank()
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
struct extent<experimental::array_ref<Params...>, ND>
  : extent<typename experimental::array_ref<Params...>::dimensions, ND> {};

namespace experimental { namespace detail
{

///////////////////////////////////////////////////////////////////////////////

// Base case.
template <>
struct count_dynamic_dims<> : std::integral_constant<std::size_t, 0> {};

template <std::size_t Head, std::size_t... Tail>
struct count_dynamic_dims<Head, Tail...>
  : std::integral_constant<std::size_t,
        ( Head == dyn
        ? count_dynamic_dims<Tail...>::value + 1
        : count_dynamic_dims<Tail...>::value) 
    > {};

///////////////////////////////////////////////////////////////////////////////

template <std::size_t... Dims>
struct make_dynamic_dims_array
{
    using type = std::array<std::size_t, count_dynamic_dims<Dims...>::value>;
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
struct pack_is_integral<> : std::true_type {};

template <typename Head, typename... Tail>
struct pack_is_integral<Head, Tail...>
  : experimental::conjunction<is_integral<Head>, pack_is_integral<Tail...> > {};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_integral_range_slice_specifier : std::false_type {};

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
  : std::true_type {};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct is_slice_specifier
  : experimental::disjunction<
        is_integral_range_slice_specifier<T>
      , is_integral<typename decay<T>::type>
    >
 {};

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

