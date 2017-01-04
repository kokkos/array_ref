///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_A3B0F119_C38A_44E3_95E4_DD40594F4328)
#define STD_A3B0F119_C38A_44E3_95E4_DD40594F4328

///////////////////////////////////////////////////////////////////////////////
// Order Agnostic Index and Stride Calculations for Regular Layouts
///////////////////////////////////////////////////////////////////////////////

// d   := dims
// p   := padding
// s   := stepping
// i   := index
// rto := rank index to ordering (smallest to largest stride dimension)
// otr := ordering to rank index (smallest to largest rank index, user provided)
// 
// stride[otr[0]] = p[otr[0]] + s[otr[0]]
// stride[n]      = p[n]      + s[n]     d[otr[rto[n]-1]]stride[otr[rto[n]-1]]
// start at n = otr[n]
// 
// index[otr[0]] = i[otr[0]]stride[otr[0]]
// index[n]      = i[n]     stride[n]      + index[otr[rto[n]-1]]
// start at n = otr[n]

#include "detail/fwd.hpp"
#include "detail/meta.hpp"
#include "detail/dimensions.hpp"

namespace std { namespace experimental { namespace detail {

///////////////////////////////////////////////////////////////////////////////

template <
    std::size_t N
  , typename Dimensions
  , typename Stepping
  , typename Padding
  , typename Ordering
  , typename enable = void
    >
struct layout_regular_indexer;

// Recursive case.
template <
    std::size_t N
  , typename Dimensions
  , typename Stepping
  , typename Padding
  , typename Ordering
  , typename enable
    >
struct layout_regular_indexer
{
    static_assert(
        (0                 <= N) 
      , "Dimension rank N is negative in layout_regular_indexer."
    );
    static_assert(
        Dimensions::rank() >  N
      , "Dimension rank N is greater than the rank of Dimensions "
        "(out of bounds) in layout_regular_indexer."
    );

    using size_type = typename Dimensions::size_type; 

    using otr_type = Ordering;
    using rto_type = make_integer_sequence_inverse_mapping<Ordering>;

    // Order-to-rank mapping. Returns the rank of the dimension with the
    // specified ordering.
    static constexpr size_type otr(size_type order) noexcept
    {
        return integer_sequence_array<otr_type>()[order];
    }

    // Rank-to-order mapping. Returns the ordering for the dimension with the
    // specified rank.
    static constexpr size_type rto(size_type rank) noexcept
    {
        return integer_sequence_array<rto_type>()[rank];
    }

    // Returns true if the Nth rank is the unit stride dimension.
    static constexpr bool is_unit_stride() noexcept
    {
        return false;
    }

    // The next type alias is not defined when false == is_unit_stride().
    using next = layout_regular_indexer<
        otr(rto(N) - 1)
      , Dimensions, Stepping, Padding, Ordering
    >;

    static constexpr bool is_dynamic_stride() noexcept
    {
        return Padding::is_dynamic(N) || Stepping::is_dynamic(N)
            || Dimensions::is_dynamic(otr(rto(N) - 1))
            || next::is_dynamic_stride();
    }

    static constexpr size_type stride(
        Dimensions             d
      , Stepping               s
      , Padding                p
        ) noexcept
    {
        return p[N] + s[N] * d[otr(rto(N) - 1)] * next::stride(d, s, p);
    }

    template <std::size_t... IdxDims>
    static constexpr size_type index(
        Dimensions             d
      , Stepping               s
      , Padding                p
      , dimensions<IdxDims...> i
        ) noexcept
    {
        return i[N] * stride(d, s, p) + next::index(d, s, p, i);
    }
};

// Base case: N == otr[0].
// Also handles the empty case (0 == Dimensions::rank()).
template <
    std::size_t N
  , typename Dimensions
  , typename Stepping
  , typename Padding
  , typename Ordering
    >
struct layout_regular_indexer<
    N
  , Dimensions
  , Stepping
  , Padding
  , Ordering
  , typename enable_if<
        is_rank_unit_stride<N, Dimensions, Ordering>::value // N == otr[0]
    >::type
>
{
    static_assert(
        (0                  <= N) 
      , "Dimension rank N is negative in layout_regular_indexer."
    );
    static_assert(
        (Dimensions::rank() >  N)
     || (0 == N && 0 == Dimensions::rank()) // Disable for the empty case.
      , "Dimension rank N is greater than the rank of Dimensions "
        "(out of bounds) in layout_regular_indexer."
    );

    using size_type = typename Dimensions::size_type; 

    using otr_type = Ordering;
    using rto_type = make_integer_sequence_inverse_mapping<Ordering>;

    // Order-to-rank mapping. Returns the rank of the dimension with the
    // specified ordering.
    //
    // Preconditions: order < Ordering::size()
    static constexpr size_type otr(size_type order) noexcept
    {
        return integer_sequence_array<otr_type>()[order];
    }

    // Rank-to-order mapping. Returns the ordering for the dimension with the
    // specified rank.
    //
    // Preconditions: rank < Dimensions::rank()
    static constexpr size_type rto(size_type rank) noexcept
    {
        return integer_sequence_array<rto_type>()[rank];
    }

    // Returns true if the Nth rank is the unit stride dimension.
    static constexpr bool is_unit_stride() noexcept
    {
        return true;
    }

    // The next type alias is not defined when false == is_unit_stride().

    static constexpr bool is_dynamic_stride() noexcept
    {
        return Padding::is_dynamic(N) || Stepping::is_dynamic(N);
    }

    static constexpr size_type stride(
        Dimensions             d
      , Stepping               s
      , Padding                p
        ) noexcept
    {
        return p[N] + s[N];
    }

    template <std::size_t... IdxDims>
    static size_type index(
        Dimensions             d
      , Stepping               s
      , Padding                p
      , dimensions<IdxDims...> i
        ) noexcept
    {
        return i[N] * stride(d, s, p);
    }
};

// TODO

template <
    typename Dimensions
  , typename Stepping
  , typename Padding
  , typename Ordering
  , typename Ranks
    >
struct make_layout_regular_striding_impl;

template <
    typename Dimensions
  , typename Stepping
  , typename Padding
  , typename Ordering
  , std::size_t... RankIndices
    >
struct make_layout_regular_striding_impl<
    Dimensions
  , Stepping
  , Padding
  , Ordering
  , index_sequence<RankIndices...>
>
{
    static_assert(
        Dimensions::rank() == sizeof...(RankIndices)
      , "The ranks of Dimensions and index_sequence<RankIndices...> are "
        "not equal."
    );

    using type = dimensions<
        ( layout_regular_indexer<
            RankIndices, Dimensions, Stepping, Padding, Ordering
          >::is_dynamic_stride()
        ? dyn
        : layout_regular_indexer<
            RankIndices, Dimensions, Stepping, Padding, Ordering
          >::stride(Dimensions{}, Stepping{}, Padding{})
        )...
    >;
};

}}} // std::experimental::detail

#endif // STD_A3B0F119_C38A_44E3_95E4_DD40594F4328

