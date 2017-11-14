///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_836A92EF_4885_4F3B_A7E1_5395FEDD3125)
#define STD_836A92EF_4885_4F3B_A7E1_5395FEDD3125

#include "detail/fwd.hpp"
#include "detail/integer_sequence.hpp"
#include "detail/dimensions.hpp"

//#warning Rename to layout_right_mapping

namespace std { namespace experimental {

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Stepping
  , typename Padding
    >
struct layout_mapping_right : Dimensions
{
    static_assert(
           (Dimensions::rank() == Stepping::rank())
        && (Stepping::rank()   == Padding::rank())
      , "The ranks of Dimensions, Stepping and Padding are not equal."
    );

    ///////////////////////////////////////////////////////////////////////////
    // TYPES

    using size_type = typename Dimensions::size_type; 

    // Smallest stride dimension to largest stride dimension (order to rank).
    using order = detail::make_reversed_index_sequence<Dimensions::rank()>;

    ///////////////////////////////////////////////////////////////////////////
    // CONSTRUCTORS AND ASSIGNMENT OPERATORS

    constexpr layout_mapping_right() noexcept;

    constexpr layout_mapping_right(layout_mapping_right const& b) noexcept = default;
    constexpr layout_mapping_right(layout_mapping_right&& b) noexcept = default;
    layout_mapping_right& operator=(layout_mapping_right const& b) noexcept = default;
    layout_mapping_right& operator=(layout_mapping_right&& b) noexcept = default;

    template <typename... DynamicDims>
    constexpr layout_mapping_right(DynamicDims... ddims) noexcept;

    constexpr layout_mapping_right(
        Dimensions d
        ) noexcept;

    constexpr layout_mapping_right(
        Dimensions d, Stepping stepping, Padding pad
        ) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    // DOMAIN SPACE

    static constexpr bool is_regular() noexcept;

    constexpr size_type stride(size_type rank) const noexcept;

    constexpr size_type span() const noexcept;

    constexpr Stepping stepping() const noexcept;

    constexpr Padding padding() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    // INDEXING

    template <typename... Idx>
    size_type index(Idx... idx) const noexcept;

  private:
    Stepping step_;
    Padding pad_;
};

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr
layout_mapping_right<Dimensions, Stepping, Padding>::layout_mapping_right() noexcept
  : Dimensions()
  , step_()
  , pad_()
{}

template <typename Dimensions, typename Stepping, typename Padding>
template <typename... DynamicDims>
inline constexpr
layout_mapping_right<Dimensions, Stepping, Padding>::layout_mapping_right(
    DynamicDims... ddims
    ) noexcept
  : Dimensions(ddims...)
  , step_()
  , pad_()
{}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr
layout_mapping_right<Dimensions, Stepping, Padding>::layout_mapping_right(
    Dimensions d
    ) noexcept
  : Dimensions(d)
  , step_()
  , pad_()
{}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr
layout_mapping_right<Dimensions, Stepping, Padding>::layout_mapping_right(
    Dimensions d, Stepping step, Padding pad
    ) noexcept
  : Dimensions(d)
  , step_(step)
  , pad_(pad)
{}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr bool
layout_mapping_right<Dimensions, Stepping, Padding>::is_regular() noexcept
{
    return true;
}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr typename
    layout_mapping_right<Dimensions, Stepping, Padding>::size_type
layout_mapping_right<Dimensions, Stepping, Padding>::stride(
    size_type rank
    ) const noexcept
{
    //#warning I think this is wrong, it should be the actual stride, right?
    return step_[rank];
}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr typename
    layout_mapping_right<Dimensions, Stepping, Padding>::size_type
layout_mapping_right<Dimensions, Stepping, Padding>::span() const noexcept
{
    return detail::dims_ternary_reduction<
        detail::span_by_value
      , detail::multiplies_by_value
      , detail::static_sentinel<1>
      , 0
      , Dimensions::rank()
    >()(*static_cast<Dimensions const*>(this), step_, pad_);
}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr Stepping
layout_mapping_right<Dimensions, Stepping, Padding>::stepping() const noexcept
{
    return step_;
}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr Padding
layout_mapping_right<Dimensions, Stepping, Padding>::padding() const noexcept
{
    return pad_;
}

template <typename Dimensions, typename Stepping, typename Padding>
template <typename... Idx>
inline typename Dimensions::size_type
layout_mapping_right<Dimensions, Stepping, Padding>::index(
    Idx... idx
    ) const noexcept
{
    // TODO: These static asserts need to actually live in mdspan. The
    // first one is particularly important, otherwise it will silently do the
    // wrong thing.
    static_assert(
        Dimensions::rank() == sizeof...(idx)
      , "Incorrect number of indices passed to layout_mapping_right."
    );

    detail::layout_mapping_right_indexer<Dimensions, Stepping, Padding, 0> indexer;
    auto i = detail::make_filled_dims_t<Dimensions::rank(), dyn>(idx...);

    return indexer(*static_cast<Dimensions const*>(this), step_, pad_, i);
}

///////////////////////////////////////////////////////////////////////////////

namespace detail {

// Recursive row-major layout implementation.
//
// Three initial cases:
// * First index, 1 < rank()
// * First index, 1 == rank()
// * 0 == rank()
//
// The first case (1 < rank()) recurses, with cases:
// * Nth index
// * Last index

// Nth index
template <
    typename Dimensions, typename Stepping, typename Padding
  , std::size_t N
  , typename enable
    >
struct layout_mapping_right_indexer
{
    template <std::size_t... IdxDims>
    typename layout_mapping_right<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        static_assert(
            (0                 <= N) 
          , "Dimension rank N is negative in layout_mapping_right."
        );
        static_assert(
            Dimensions::rank() >  N
          , "Dimension rank N is greater than the rank of Dimensions "
            "(out of bounds) in layout_mapping_right."
        );
        layout_mapping_right_indexer<Dimensions, Stepping, Padding, N + 1> const
            next;
        auto constexpr R = Dimensions::rank();
        return (d[R - N] * step[R - N] + pad[R - N])
             * (step[(R - 1) - N] * i[(R - 1) - N] + next(d, step, pad, i));
    }
};

// First index, 1 < rank()
template <
    typename Dimensions
  , typename Stepping
  , typename Padding
    >
struct layout_mapping_right_indexer<
    Dimensions
  , Stepping
  , Padding
  , 0                                                       // First index
  , typename enable_if<
        detail::is_rank_greater_than<1, Dimensions>::value     // 1 < rank()
    >::type 
>
{
    template <std::size_t... IdxDims>
    typename layout_mapping_right<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        layout_mapping_right_indexer<Dimensions, Stepping, Padding, 1> const
            next;
        auto constexpr R = Dimensions::rank();
        return step[R - 1] * i[R - 1] + next(d, step, pad, i);
    }
};

// First index, 1 == rank()
template <
    typename Dimensions
  , typename Stepping
  , typename Padding
    >
struct layout_mapping_right_indexer<
    Dimensions
  , Stepping
  , Padding
  , 0                                                       // First index
  , typename enable_if<
        detail::is_rank_equal_to<1, Dimensions>::value         // 1 == rank()
    >::type 
>
{
    template <std::size_t... IdxDims>
    typename layout_mapping_right<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        auto constexpr R = Dimensions::rank();
        return step[R - 1] * i[R - 1];
    }
};

// 0 == rank()
template <
    typename Dimensions
  , typename Stepping
  , typename Padding
  , std::size_t N
    >
struct layout_mapping_right_indexer<
    Dimensions
  , Stepping
  , Padding
  , N
  , typename enable_if<
        detail::is_rank_equal_to<0, Dimensions>::value         // 0 == rank()
    >::type 
>
{
    template <std::size_t... IdxDims>
    typename layout_mapping_right<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        return 0;
    }
};

// Last index
template <
    typename Dimensions
  , typename Stepping
  , typename Padding
  , std::size_t N
    >
struct layout_mapping_right_indexer<
    Dimensions
  , Stepping
  , Padding
  , N
  , typename enable_if<
        detail::is_rank_last_index<N, Dimensions>::value         // Last index
    >::type 
>
{
    template <std::size_t... IdxDims>
    typename layout_mapping_right<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        static_assert(
            (0                 <= N) 
          , "Dimension rank N is negative in layout_mapping_right."
        );
        static_assert(
            Dimensions::rank() >  N
          , "Dimension rank N is greater than the rank of Dimensions "
            "(out of bounds) in layout_mapping_right."
        );
        auto constexpr R = Dimensions::rank();
        return (d[R - N] * step[R - N] + pad[R - N])
             * (step[(R - 1) - N] * i[(R - 1) - N]);
    }
};

}}} // std::experimental::detail

#endif // STD_836A92EF_4885_4F3B_A7E1_5395FEDD3125

