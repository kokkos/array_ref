///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_FD8D1DFC_4745_40CA_A752_6E6AC86038D4)
#define STD_FD8D1DFC_4745_40CA_A752_6E6AC86038D4

#include <cstdint>

namespace std { namespace experimental { namespace detail
{

template <std::size_t... Dims>
struct dimensions_impl;

///////////////////////////////////////////////////////////////////////////////

// Runtime implementation of the std::extent metafunction. 
template <typename Idx>
inline std::size_t constexpr dynamic_extent(
    Idx idx
    ) noexcept;

template <typename Idx, typename Head, typename... Tail>
inline std::size_t constexpr dynamic_extent(
    Idx idx, Head head, Tail... tail
    ) noexcept;

// Maps a dimension index referring to a dynamic index (idx) to an index in the
// dynamic dimension array. E.g. if you have dimensions<3, 0, 4, 0>, this
// metafunction would map 1 to 0 (the first dynamic dimension) and 3 to 1 (the
// second one).
// Base case.
template <typename Idx>
inline constexpr std::size_t index_into_dynamic_dims(
    Idx idx
    ) noexcept;

template <typename Idx, typename Head, typename... Tail>
inline constexpr std::size_t index_into_dynamic_dims(
    Idx idx, Head head, Tail... tail
    ) noexcept;

///////////////////////////////////////////////////////////////////////////////

// Metafunction which maps 0 to dynamic_dimension, and returns the identity
// otherwise. This is used to allow 0 to be used to specify dynamic dimensions,
// without the extent-related complications of actually making dynamic_dimension
// 0.
template <std::size_t Dim>
struct replace_0_with_dynamic_dimension;

// Counts the number of dynamic dimensions.
template <std::size_t... Dims>
struct count_dynamic_dims;

// Builds a std::array with one entry for each dynamic dimension.
template <std::size_t... Dims>
struct build_dims_array;

// Metafunction which returns true if std::is_integral<> is true for all of the
// types in the parameter pack.
template <typename... T>
struct pack_is_integral;

} // detail

///////////////////////////////////////////////////////////////////////////////

constexpr std::size_t dynamic_dimension = -1;

template <std::size_t... Dims>
using dimensions = detail::dimensions_impl<
    detail::replace_0_with_dynamic_dimension<Dims>::value...
>;

///////////////////////////////////////////////////////////////////////////////

template <typename ValueType, typename Dims, typename Traits>
struct array_ref;

///////////////////////////////////////////////////////////////////////////////

template <typename Striding, typename Padding>
struct basic_layout_left;

// TODO
template <typename Striding, typename Padding>
struct basic_layout_right; 

// TODO
template <typename Striding, typename Padding, typename Ordering>
struct basic_layout_order;

// TODO
struct layout_left;

// TODO
struct layout_right; 

// TODO
template <std::size_t... Ordering>
struct layout_order;

}} // std::experimental

#endif // STD_FD8D1DFC_4745_40CA_A752_6E6AC86038D4

