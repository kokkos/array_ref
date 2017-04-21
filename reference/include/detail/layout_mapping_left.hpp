///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_0F383687_D414_463B_9C79_74EA4545EEB5)
#define STD_0F383687_D414_463B_9C79_74EA4545EEB5

#include "detail/fwd.hpp"
#include "detail/integer_sequence.hpp"
#include "detail/dimensions.hpp"

//#warning Add and use an embedded dimensions typedef

//#warning Better coverage for stepping() and padding()

namespace std { namespace experimental {

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Stepping
  , typename Padding
    >
struct layout_mapping_left : Dimensions
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
    using order = detail::make_index_sequence<Dimensions::rank()>;

    ///////////////////////////////////////////////////////////////////////////
    // CONSTRUCTORS AND ASSIGNMENT OPERATORS

    constexpr layout_mapping_left() noexcept;

    constexpr layout_mapping_left(layout_mapping_left const& b) noexcept
        = default;
    constexpr layout_mapping_left(layout_mapping_left&& b) noexcept
        = default;
    layout_mapping_left& operator=(layout_mapping_left const& b) noexcept
        = default;
    layout_mapping_left& operator=(layout_mapping_left&& b) noexcept
        = default;

    template <typename... DynamicDims>
    constexpr layout_mapping_left(DynamicDims... ddims) noexcept;

    constexpr layout_mapping_left(
        Dimensions d
        ) noexcept;

    constexpr layout_mapping_left(
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
layout_mapping_left<Dimensions, Stepping, Padding>::layout_mapping_left() noexcept
  : Dimensions()
  , step_()
  , pad_()
{}

template <typename Dimensions, typename Stepping, typename Padding>
template <typename... DynamicDims>
inline constexpr
layout_mapping_left<Dimensions, Stepping, Padding>::layout_mapping_left(
    DynamicDims... ddims
    ) noexcept
  : Dimensions(ddims...)
  , step_()
  , pad_()
{}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr
layout_mapping_left<Dimensions, Stepping, Padding>::layout_mapping_left(
    Dimensions d
    ) noexcept
  : Dimensions(d)
  , step_()
  , pad_()
{}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr
layout_mapping_left<Dimensions, Stepping, Padding>::layout_mapping_left(
    Dimensions d, Stepping step, Padding pad
    ) noexcept
  : Dimensions(d)
  , step_(step)
  , pad_(pad)
{}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr bool
layout_mapping_left<Dimensions, Stepping, Padding>::is_regular() noexcept
{
    return true;
}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr typename
    layout_mapping_left<Dimensions, Stepping, Padding>::size_type
layout_mapping_left<Dimensions, Stepping, Padding>::stride(
    size_type rank
    ) const noexcept
{
    //#warning I think this is wrong, it should be the actual stride, right?
    return step_[rank];
}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr typename
    layout_mapping_left<Dimensions, Stepping, Padding>::size_type
layout_mapping_left<Dimensions, Stepping, Padding>::span() const noexcept
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
layout_mapping_left<Dimensions, Stepping, Padding>::stepping() const noexcept
{
    return step_;
}

template <typename Dimensions, typename Stepping, typename Padding>
inline constexpr Padding
layout_mapping_left<Dimensions, Stepping, Padding>::padding() const noexcept
{
    return pad_;
}

template <typename Dimensions, typename Stepping, typename Padding>
template <typename... Idx>
inline typename Dimensions::size_type
layout_mapping_left<Dimensions, Stepping, Padding>::index(
    Idx... idx
    ) const noexcept
{
    // TODO: These static asserts need to actually live in mdspan. The
    // first one is particularly important, otherwise it will silently do the
    // wrong thing.
    static_assert(
        Dimensions::rank() == sizeof...(idx)
      , "Incorrect number of indices passed to layout_mapping_left."
    );

    detail::layout_mapping_left_indexer<Dimensions, Stepping, Padding, 0> indexer;
    auto i = detail::make_filled_dims_t<Dimensions::rank(), dyn>(idx...);

    return indexer(*static_cast<Dimensions const*>(this), step_, pad_, i);
}

///////////////////////////////////////////////////////////////////////////////

namespace detail {

// Recursive column-major layout implementation.
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
struct layout_mapping_left_indexer
{
    template <std::size_t... IdxDims>
    typename layout_mapping_left<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        static_assert(
            (0                 <= N) 
          , "Dimension rank N is negative in layout_mapping_left."
        );
        static_assert(
            Dimensions::rank() >  N
          , "Dimension rank N is greater than the rank of Dimensions "
            "(out of bounds) in layout_mapping_left."
        );
        layout_mapping_left_indexer<Dimensions, Stepping, Padding, N + 1> const
            next;
        return (d[N - 1] * step[N - 1] + pad[N - 1])
             * (step[N] * i[N] + next(d, step, pad, i));
    }
};

// First index, 1 < rank()
template <
    typename Dimensions
  , typename Stepping
  , typename Padding
    >
struct layout_mapping_left_indexer<
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
    typename layout_mapping_left<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        layout_mapping_left_indexer<Dimensions, Stepping, Padding, 1>
            const next;
        return step[0] * i[0] + next(d, step, pad, i);
    }
};

// First index, 1 == rank()
template <
    typename Dimensions
  , typename Stepping
  , typename Padding
    >
struct layout_mapping_left_indexer<
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
    typename layout_mapping_left<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        return step[0] * i[0];
    }
};

// 0 == rank()
template <
    typename Dimensions
  , typename Stepping
  , typename Padding
  , std::size_t N
    >
struct layout_mapping_left_indexer<
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
    typename layout_mapping_left<Dimensions, Stepping, Padding>::size_type
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
struct layout_mapping_left_indexer<
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
    typename layout_mapping_left<Dimensions, Stepping, Padding>::size_type
    operator()(
        Dimensions d
      , Stepping step
      , Padding pad
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        static_assert(
            (0                 <= N) 
          , "Dimension rank N is negative in layout_mapping_left."
        );
        static_assert(
            Dimensions::rank() >  N
          , "Dimension rank N is greater than the rank of Dimensions "
            "(out of bounds) in layout_mapping_left."
        );
        return (d[N - 1] * step[N - 1] + pad[N - 1])
             * (step[N] * i[N]);
    }
};

}}} // std::experimental::detail

#endif // STD_0F383687_D414_463B_9C79_74EA4545EEB5

