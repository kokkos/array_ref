///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_FD8D1DFC_4745_40CA_A752_6E6AC86038D4)
#define STD_FD8D1DFC_4745_40CA_A752_6E6AC86038D4

#include <cstdint>

namespace std { namespace experimental { namespace detail {

///////////////////////////////////////////////////////////////////////////////

// C++17's logical type traits.

// Metafunction returning T0::value && T1::value && ... && TN::value. Short
// circuits so that if TK::value is false, TKplus1::value is not instantiated.
template <typename... Ts>
struct conjunction;

// Metafunction returning T0::value || T1::value || ... || TN::value. 
template <typename... Ts>
struct disjunction;

// Metafunction returning !T::value
template <typename T>
struct negation;

///////////////////////////////////////////////////////////////////////////////

// Subset of C++14's type trait *_t type aliases.

// Metafunction which returns True if B and False otherwise.
template <bool B, typename True, typename False>
using conditional_t = typename conditional<B, True, False>::type;

///////////////////////////////////////////////////////////////////////////////

// C++14's integer_sequence and associated helper aliases plus some non-standard
// extensions.

// A compile-time sequence of integral constants of type T.
template <typename T, T... I>
struct integer_sequence;

// A compile-time sequence of std::size_t constants.
template <std::size_t... Idxs>
using index_sequence = integer_sequence<std::size_t, Idxs...>;

// Create a new integer_sequence containing the elements of Sequence0 followed
// by the elements of Sequence1. Sequence0::size() is added to each element from
// Sequence1 in the new sequence.
template <typename Sequence0, typename Sequence1>
  struct merge_and_renumber_integer_sequences_impl;
template <typename Sequence0, typename Sequence1>
  using merge_and_renumber_integer_sequences =
      typename merge_and_renumber_integer_sequences_impl<
          Sequence0, Sequence1
      >::type;

// Create a new integer_sequence containing the elements of Sequence0 followed
// by the elements of Sequence1. Sequence1::size() is added to each element from
// Sequence0 in the new sequence.
template <typename Sequence0, typename Sequence1>
  struct merge_and_renumber_reversed_integer_sequences_impl;
template <typename Sequence0, typename Sequence1>
  using merge_and_renumber_reversed_integer_sequences =
      typename merge_and_renumber_reversed_integer_sequences_impl<
          Sequence0, Sequence1
      >::type;

// Create a new integer_sequence with elements 0, 1, 2, ..., N - 1.
template <typename T, std::size_t N>
  struct make_integer_sequence_impl;
template <typename T, std::size_t N>
  using make_integer_sequence =
    typename make_integer_sequence_impl<T, N>::type;

// Create a new index_sequence with elements 0, 1, 2, ..., N - 1.
template <std::size_t N>
  using make_index_sequence =
    make_integer_sequence<std::size_t, N>;

// Create a new integer_sequence with elements N - 1, N - 2, N - 3, ..., 0.
template <typename T, std::size_t N>
  struct make_reversed_integer_sequence_impl;
template <typename T, std::size_t N>
  using make_reversed_integer_sequence =
    typename make_reversed_integer_sequence_impl<T, N>::type;

// Create a new index_sequence with elements N - 1, N - 2, N - 3, ..., 0.
template <std::size_t N>
  using make_reversed_index_sequence =
    make_reversed_integer_sequence<std::size_t, N>;

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

// Initialize dynamic dims storage from a parameter pack of dims that include
// static dims.

// Base case.
template <typename Idx, typename DynamicDimsArray>
inline constexpr DynamicDimsArray filter_initialize_dynamic_dims_array(
    Idx              idx
  , DynamicDimsArray a
    ) noexcept;

template <
    std::size_t    DimsHead
  , std::size_t... DimsTail
  , typename       Idx
  , typename       DynamicDimsArray
  , typename       DynamicDimsHead
  , typename...    DynamicDimsTail
    >
inline constexpr DynamicDimsArray filter_initialize_dynamic_dims_array(
    Idx                idx
  , DynamicDimsArray   a
  , DynamicDimsHead    head
  , DynamicDimsTail... tail
    ) noexcept;

///////////////////////////////////////////////////////////////////////////////

// A compile-time sequence of types.

template <typename... T>
struct type_list;

///////////////////////////////////////////////////////////////////////////////

// A compile-time data structure which stores the contents of an
// integer_sequence<> in a static constexpr array and provides a fast constexpr
// indexing operation.

template <typename Sequence>
struct integer_sequence_array;

///////////////////////////////////////////////////////////////////////////////

// Compile-time pair data structure.

template <typename Key, typename Value, Key K, Value V>
struct integral_pair;

///////////////////////////////////////////////////////////////////////////////

// Compile-time sorting predicates.

// Metafunction class with an ambedded apply<T0, T1> metafunction that returns
// true if T0::key is less than T1::key. 
struct type_key_less;

// Metafunction class with an embedded apply<T0, T1> metafunction that returns
// true if T0::value is less than T1::value. 
struct type_value_less;

///////////////////////////////////////////////////////////////////////////////

// Compile-time insertion operations.

// Add a new dimension to the front of a dimensions<>.
template <std::size_t Dim, typename Dimensions> 
  struct dimensions_push_front_impl;
template <std::size_t Dim, typename Dimensions> 
  using dimensions_push_front =
    typename dimensions_push_front_impl<Dim, Dimensions>::type;

// Add a new dimension to the back of a dimensions<>.
template <std::size_t Dim, typename Dimensions> 
  struct dimensions_push_back_impl;
template <std::size_t Dim, typename Dimensions> 
  using dimensions_push_back =
    typename dimensions_push_back_impl<Dim, Dimensions>::type;

// Add a new element to the front of an integer_sequence<>.
template <typename T, T I, typename Sequence> 
  struct integer_sequence_push_front_impl;
template <typename T, T I, typename Sequence> 
  using integer_sequence_push_front =
    typename integer_sequence_push_front_impl<T, I, Sequence>::type;

// Add a new element to the back of an integer_sequence<>.
template <typename T, T I, typename Sequence> 
  struct integer_sequence_push_back_impl;
template <typename T, T I, typename Sequence> 
  using integer_sequence_push_back =
    typename integer_sequence_push_back_impl<T, I, Sequence>::type;

// Add a new element to the front of a type_list<>.
template <typename T, typename Sequence> 
  struct type_list_push_front_impl;
template <typename T, typename Sequence> 
  using type_list_push_front =
    typename type_list_push_front_impl<T, Sequence>::type;

// Add a new element to the back of a type_list<>.
template <typename T, typename Sequence> 
  struct type_list_push_back_impl;
template <typename T, typename Sequence> 
  using type_list_push_back =
    typename type_list_push_back_impl<T, Sequence>::type;

///////////////////////////////////////////////////////////////////////////////

// Add a new element to a sorted type_list<>. Compare is a metafunction class;
// its embedded apply<T0, T1> template takes two parameters and returns a
// boolean integral_constant<>.
template <typename T, typename Sequence, typename Compare = type_value_less> 
  struct type_list_push_impl;
template <typename T, typename Sequence, typename Compare = type_value_less> 
  using type_list_push = 
    typename type_list_push_impl<T, Sequence, Compare>::type;

// Creates a sorted type_list<> from an input type_list<>. Compare is a
// metafunction class; its embedded apply<T0, T1> template takes two parameters
// and returns a boolean integral_constant<>.
template <typename Sequence, typename Compare = type_value_less> 
  struct type_list_sort_impl;
template <typename Sequence, typename Compare = type_value_less> 
  using type_list_sort = typename type_list_sort_impl<Sequence, Compare>::type;

///////////////////////////////////////////////////////////////////////////////

// Creates a typelist of integral_pairs from two input integer_sequence<>s.
template <typename KeySequence, typename ValueSequence>
  struct make_key_value_type_list_impl;
template <typename KeySequence, typename ValueSequence>
  using make_key_value_type_list =
    typename make_key_value_type_list_impl<KeySequence, ValueSequence>::type; 

// Creates a typelist of integral_pairs from an integer_sequence<>, where
// the key is the position of the value in the input integer_sequence<>.
template <typename Sequence>
  struct make_key_value_type_list_from_integer_sequence_impl;
template <typename Sequence>
  using make_key_value_type_list_from_integer_sequence =
    typename make_key_value_type_list_from_integer_sequence_impl<
        Sequence
    >::type;

// Creates an integer_sequence<> from the keys of a type_list<> of
// integral_pair<>s.
template <typename T, typename KeyValueSequence>
  struct make_integer_sequence_from_keys_impl;
template <typename T, typename KeyValueSequence>
  using make_integer_sequence_from_keys =
    typename make_integer_sequence_from_keys_impl<T, KeyValueSequence>::type;

// Creates an integer_sequence<> which maps the values of the input
// integer_sequence<> to the positions of those values.
template <typename Sequence>
  struct make_integer_sequence_inverse_mapping_impl;
template <typename Sequence>
  using make_integer_sequence_inverse_mapping =
    typename make_integer_sequence_inverse_mapping_impl<Sequence>::type;

///////////////////////////////////////////////////////////////////////////////
// Expression SFINAE workarounds for MSVC.

template <std::size_t N, typename Dimensions>
struct is_rank_greater_than;

template <std::size_t N, typename Dimensions>
struct is_rank_equal_to;

template <std::size_t N, typename Dimensions, typename Ordering>
struct is_rank_unit_stride;

template <std::size_t N, typename Dimensions>
struct is_rank_last_index;

///////////////////////////////////////////////////////////////////////////////

// Counts the number of dynamic dimensions.
template <std::size_t... Dims>
struct count_dynamic_dims;

// Create an integer_sequence<> of the rank indices of dynamic dimensions.
template <std::size_t N, std::size_t... Dims>
  struct make_dynamic_dims_indices_impl;
template <std::size_t... Dims>
  using make_dynamic_dims_indices =
    typename make_dynamic_dims_indices_impl<0, Dims...>::type;

// Builds an array<> with one entry for each dynamic dimension.
template <std::size_t... Dims>
struct make_dynamic_dims_array;

template <std::size_t... Dims>
using make_dynamic_dims_array_t =
    typename make_dynamic_dims_array<Dims...>::type;

// Creates a dimensions<> type of rank N with static extents of Value.
template <std::size_t N, std::size_t Value, std::size_t... Dims>
struct make_filled_dims;

template <std::size_t N, std::size_t Value, std::size_t... Dims>
using make_filled_dims_t = typename make_filled_dims<N, Value, Dims...>::type;

///////////////////////////////////////////////////////////////////////////////

// Returns true if std::is_integral<> is true for all of the types in the
// parameter pack.
template <typename... T>
struct is_integral_pack;

// Returns true if T is an integral range slice specifier, e.g. one of the
// following, where I0 and I1 are integral types:
// * initializer_list<I0>
// * pair<I0, I1>
// * tuple<I0, I1>
// * array<I0, 2>
// * all (denoting the entire dimension)
template <typename T>
struct is_integral_range_slice_specifier;

// Returns true if T is a slice specifier.
template <typename T>
struct is_slice_specifier;

// Returns true if is_slice_specifier<> is true for all of the types in the
// parameter pack.
template <typename... T>
struct pack_is_slice_specifiers;

// Counts the number of dynamic dimensions.
template <typename... Dims>
struct count_integral_range_slice_specifiers;

///////////////////////////////////////////////////////////////////////////////

// Function-object recursive implementation of layout_left indexing. N is the
// rank in the index dimensions<> object.
template <
    typename Dimensions, typename Striding, typename Padding
  , std::size_t N
  , typename enable = void
    >
struct layout_mapping_left_indexer;

// Function-object recursive implementation of layout_right indexing. N is the
// rank in the index dimensions<> object.
template <
    typename Dimensions, typename Striding, typename Padding
  , std::size_t N
  , typename enable = void
    >
struct layout_mapping_right_indexer;

} // std::experimental::detail

///////////////////////////////////////////////////////////////////////////////

// Produces a dimensions<> object from a native array declaration.
template <typename T, std::size_t... Dims>
struct extract_dimensions;

template <typename T>
using extract_dimensions_t = typename extract_dimensions<T>::type;

///////////////////////////////////////////////////////////////////////////////

namespace detail {

struct all_tag {};

} // std::experimental::detail 

constexpr detail::all_tag all{};

///////////////////////////////////////////////////////////////////////////////

constexpr std::size_t dyn = -1;

template <std::size_t... Dims>
struct dimensions;

///////////////////////////////////////////////////////////////////////////////

// TODO: Specify ArrayRefLayout concept which these classes implement.

template <
    typename Dimensions
  , typename Striding
  , typename Padding
    >
struct layout_mapping_left;

template <
    typename Dimensions
  , typename Striding
  , typename Padding
    >
struct layout_mapping_right; 

struct layout_left;

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
struct mdspan;

}} // std::experimental

#endif // STD_FD8D1DFC_4745_40CA_A752_6E6AC86038D4

