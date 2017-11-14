///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_DFA2020D_4C7F_4C76_BCA2_668E25E9FAE4)
#define STD_DFA2020D_4C7F_4C76_BCA2_668E25E9FAE4

#include <cstdint>
#include <type_traits>

#include "detail/fwd.hpp"

namespace std { namespace experimental { namespace detail {

///////////////////////////////////////////////////////////////////////////////

template <typename T, T... I>
struct integer_sequence
{
    using type = integer_sequence;
    using value_type = T;
    using size_type = std::size_t;

    static constexpr size_type size() noexcept
    {
        return sizeof...(I);
    }
};

///////////////////////////////////////////////////////////////////////////////

template <typename T, T... I0, T... I1>
struct merge_and_renumber_integer_sequences_impl<
           integer_sequence<T, I0...>, integer_sequence<T, I1...>
       >
{
    using type = integer_sequence<T, I0..., (sizeof...(I0) + I1)...>;
};

template <typename T, T... I0, T... I1>
struct merge_and_renumber_reversed_integer_sequences_impl<
           integer_sequence<T, I0...>, integer_sequence<T, I1...>
       >
{
    using type = integer_sequence<T, (sizeof...(I1) + I0)..., I1...>;
};

///////////////////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
struct make_integer_sequence_impl
{
    using type = merge_and_renumber_integer_sequences<
        make_integer_sequence<T, N / 2>
      , make_integer_sequence<T, N - N / 2>
    >;
};

template <typename T>
struct make_integer_sequence_impl<T, 0>
{
    using type = integer_sequence<T>;
};

template <typename T>
struct make_integer_sequence_impl<T, 1>
{
    using type = integer_sequence<T, 0>;
};

///////////////////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
struct make_reversed_integer_sequence_impl
{
    using type = merge_and_renumber_reversed_integer_sequences<
        make_reversed_integer_sequence<T, N / 2>
      , make_reversed_integer_sequence<T, N - N / 2>
    >;
};

template <typename T>
struct make_reversed_integer_sequence_impl<T, 0>
{
    using type = integer_sequence<T>;
};

template <typename T>
struct make_reversed_integer_sequence_impl<T, 1>
{
    using type = integer_sequence<T, 0>;
};

}}} // std::experimental::detail

#endif // STD_DFA2020D_4C7F_4C76_BCA2_668E25E9FAE4

