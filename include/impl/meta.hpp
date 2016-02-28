///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_324BD9FF_856B_4DC7_BC6F_2A93F3DF63CD)
#define STD_324BD9FF_856B_4DC7_BC6F_2A93F3DF63CD

#include <type_traits>

#include "impl/fwd.hpp"
#include "impl/conjunction.hpp"

namespace std
{

///////////////////////////////////////////////////////////////////////////////

template <std::size_t... Dims>
struct rank<experimental::detail::dimensions_impl<Dims...> >
  : std::integral_constant<std::size_t, sizeof...(Dims)> {};

template <std::size_t Head, std::size_t... Tail>
struct extent<experimental::detail::dimensions_impl<Head, Tail...>, 0>
  : std::integral_constant<std::size_t, Head> {};

template <std::size_t Head, std::size_t... Tail, unsigned ND>
struct extent<experimental::detail::dimensions_impl<Head, Tail...>, ND>
  : std::integral_constant<
        std::size_t
      , std::extent<
            experimental::detail::dimensions_impl<Tail...>, ND - 1
        >::value
    > {};

namespace experimental { namespace detail
{

///////////////////////////////////////////////////////////////////////////////

template <>
struct replace_0_with_dynamic_dimension<0>
  : std::integral_constant<std::size_t, dynamic_dimension> {};

template <std::size_t Dim>
struct replace_0_with_dynamic_dimension
  : std::integral_constant<std::size_t, Dim> {}; 

///////////////////////////////////////////////////////////////////////////////

// Base case.
template <>
struct count_dynamic_dims<> : std::integral_constant<std::size_t, 0> {};

template <std::size_t Head, std::size_t... Tail>
struct count_dynamic_dims<Head, Tail...>
  : std::integral_constant<std::size_t,
        ( Head == dynamic_dimension
        ? count_dynamic_dims<Tail...>::value + 1
        : count_dynamic_dims<Tail...>::value) 
    >{};

///////////////////////////////////////////////////////////////////////////////

template <std::size_t... Dims>
struct build_dims_array
{
    using type = std::array<std::size_t, count_dynamic_dims<Dims...>::value>;
};

///////////////////////////////////////////////////////////////////////////////

// Base case.
template <>
struct pack_is_integral<> : std::true_type {};

template <typename Head, typename... Tail>
struct pack_is_integral<Head, Tail...>
  : conjunction<std::is_integral<Head>, pack_is_integral<Tail...> > {};

}}} // std::experimental::detail 

#endif // STD_324BD9FF_856B_4DC7_BC6F_2A93F3DF63CD

