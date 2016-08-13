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

///////////////////////////////////////////////////////////////////////////////

// Reduction-style visitation of the extents of dimensions<>. Op, Reduction and
// Sentinel are function objects with a constexpr const method that takes the
// correct number of dimensions<> objects by value. 

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Idx
  , std::size_t Size
    >
struct dims_unary_reduction;

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Idx
  , std::size_t Size
    >
struct dims_binary_reduction;

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Idx
  , std::size_t Size
    >
struct dims_ternary_reduction;

///////////////////////////////////////////////////////////////////////////////

// <functional>-style function objects with value semantics.

struct identity_by_value;

struct multiplies_by_value;

struct span_by_value;

template <std::size_t Value> 
struct static_sentinel;

///////////////////////////////////////////////////////////////////////////////

// Runtime implementation of the std::extent metafunction. 

// Base case.
template <typename Idx>
inline std::size_t constexpr dynamic_extent(
    Idx idx
    ) noexcept;

template <typename Idx, typename Head, typename... Tail>
inline std::size_t constexpr dynamic_extent(
    Idx idx, Head head, Tail... tail
    ) noexcept;

// Maps a dimension index referring to a dynamic index (idx) to an index in the
// dynamic dimension array. E.g. if you have dimensions<3, dyn, 4, dyn>, this
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

// Counts the number of dynamic dimensions.
template <std::size_t... Dims>
struct count_dynamic_dims;

// Builds a std::array with one entry for each dynamic dimension.
template <std::size_t... Dims>
struct make_dynamic_dims_array;

template <std::size_t... Dims>
using make_dynamic_dims_array_t =
    typename make_dynamic_dims_array<Dims...>::type;

// Returns a dimensions<> object of size N with static extents of Value.
template <std::size_t N, std::size_t Value, std::size_t... Dims>
struct make_filled_dims;

template <std::size_t N, std::size_t Value, std::size_t... Dims>
using make_filled_dims_t = typename make_filled_dims<N, Value, Dims...>::type;

// Returns true if std::is_integral<> is true for all of the types in the
// parameter pack.
template <typename... T>
struct pack_is_integral;

// Returns true if std::is_unsigned<> is true for all of the types in the
// parameter pack.
template <typename... T>
struct pack_is_unsigned;

///////////////////////////////////////////////////////////////////////////////

// Function-object recursive implementation of layout_left indexing. N is the
// rank in the index dimensions<> object.
template <typename Dimensions, std::size_t N, typename enable = void>
struct basic_layout_left_indexer;

// Function-object recursive implementation of layout_right indexing. N is the
// rank in the index dimensions<> object.
template <typename Dimensions, std::size_t N, typename enable = void>
struct basic_layout_right_indexer;

} // detail

///////////////////////////////////////////////////////////////////////////////

// Produces a dimensions<> object from a native array declaration.
template <typename T, std::size_t... Dims>
struct extract_dimensions;

template <typename T>
using extract_dimensions_t = typename extract_dimensions<T>::type;

///////////////////////////////////////////////////////////////////////////////

constexpr std::size_t dyn = -1;

template <std::size_t... Dims>
struct dimensions;

///////////////////////////////////////////////////////////////////////////////

// TODO: Specify ArrayRefLayout concept which these classes implement.

template <typename Striding, typename Padding>
struct basic_layout_left;

// TODO
template <typename Striding, typename Padding>
struct basic_layout_right; 

// TODO
template <typename Striding, typename Padding, typename Ordering>
struct basic_layout_order;

struct layout_left;

// TODO
struct layout_right; 

// TODO
template <std::size_t... Ordering>
struct layout_order;

using layout_native = layout_right;

///////////////////////////////////////////////////////////////////////////////

// TODO: Specify ArrayRefAccessor concept which these classes implement.

template <typename ValueType>
struct accessor_native;

///////////////////////////////////////////////////////////////////////////////

template <typename ValueType
        , typename Dim       = extract_dimensions_t<ValueType>
        , typename Layout    = layout_native
        , typename Accessor  = accessor_native<ValueType>
         >
struct array_ref;

}} // std::experimental

#endif // STD_FD8D1DFC_4745_40CA_A752_6E6AC86038D4

