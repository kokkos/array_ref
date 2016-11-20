///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_DFA2020D_4C7F_4C76_BCA2_668E25E9FAE4)
#define STD_DFA2020D_4C7F_4C76_BCA2_668E25E9FAE4

#include <cstdint>
#include <type_traits>

#include "detail/fwd.hpp"

namespace std { namespace experimental
{

template <typename T, T... I>
struct integer_sequence
{
    using type = integer_sequence;
    using value_type = T;

    static constexpr std::size_t size() noexcept
    {
        return sizeof...(I);
    }
};

///////////////////////////////////////////////////////////////////////////////

namespace detail
{

template <typename T, T... I0, T... I1>
struct merge_and_renumber_integer_sequences<
           integer_sequence<T, I0...>, integer_sequence<T, I1...>
       >
  : integer_sequence<T, I0..., (sizeof...(I0) + I1)...> {};

template <typename T, T... I0, T... I1>
struct merge_and_renumber_reversed_integer_sequences<
           integer_sequence<T, I0...>, integer_sequence<T, I1...>
       >
  : integer_sequence<T, (sizeof...(I1) + I0)..., I1...> {};

} // std::experimental::detail

///////////////////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
struct make_integer_sequence
  : detail::merge_and_renumber_integer_sequences<
        typename make_integer_sequence<T, N / 2>::type
      , typename make_integer_sequence<T, N - N / 2>::type
    > {};

template <typename T>
struct make_integer_sequence<T, 0> : integer_sequence<T> {};

template <typename T>
struct make_integer_sequence<T, 1> : integer_sequence<T, 0> {};

///////////////////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
struct make_reversed_integer_sequence
  : detail::merge_and_renumber_reversed_integer_sequences<
        typename make_reversed_integer_sequence<T, N / 2>::type
      , typename make_reversed_integer_sequence<T, N - N / 2>::type
    > {};

template <typename T>
struct make_reversed_integer_sequence<T, 0> : integer_sequence<T> {};

template <typename T>
struct make_reversed_integer_sequence<T, 1> : integer_sequence<T, 0> {};

}} // std::experimental

#endif // STD_DFA2020D_4C7F_4C76_BCA2_668E25E9FAE4

