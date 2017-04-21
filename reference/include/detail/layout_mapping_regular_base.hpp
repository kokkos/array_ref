///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_A3B0F119_C38A_44E3_95E4_DD40594F4328)
#define STD_A3B0F119_C38A_44E3_95E4_DD40594F4328

///////////////////////////////////////////////////////////////////////////////
// Order Agnostic Regular Layout Mapping - Base Class 
///////////////////////////////////////////////////////////////////////////////

// d   := dims
// p   := pads
// s   := steps
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
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Ordering
    >
struct layout_mapping_regular_base;

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
struct layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
> : Dimensions
{
    static_assert(
        Dimensions::rank() == Steps::rank() 
      , "The ranks of Dimensions and Steps are not equal."
    );
    static_assert(
        Dimensions::rank() == Pads::rank() 
      , "The ranks of Dimensions and Pads are not equal."
    );
    static_assert(
        Dimensions::rank() == sizeof...(OrderIndices)
      , "The ranks of Dimensions and index_sequence<OrderIndices...> are "
        "not equal."
    );

    ///////////////////////////////////////////////////////////////////////////
    // TYPES

    using base_type = Dimensions;

    using typename Dimensions::value_type;
    //using size_type = typename Dimensions::size_type; 
    using typename Dimensions::size_type;

    // Smallest stride dimension to largest stride dimension (order to rank).
    using ordering = index_sequence<OrderIndices...>;

    // Inverse mapping of ordering (rank to order).
    using inverse_ordering = make_integer_sequence_inverse_mapping<ordering>;

    ///////////////////////////////////////////////////////////////////////////
    // CONSTRUCTORS AND ASSIGNMENT OPERATORS

    constexpr layout_mapping_regular_base() noexcept;

    constexpr layout_mapping_regular_base(
        layout_mapping_regular_base const&
        ) noexcept = default;
    constexpr layout_mapping_regular_base(
        layout_mapping_regular_base&&
        ) noexcept = default;
    layout_mapping_regular_base& operator=(
        layout_mapping_regular_base const&
        ) noexcept = default;
    layout_mapping_regular_base& operator=(
        layout_mapping_regular_base&&
        ) noexcept = default;

    template <typename... DynamicDims>
    constexpr layout_mapping_regular_base(DynamicDims&&... ddims) noexcept;

    template <std::size_t N>
    constexpr layout_mapping_regular_base(array<value_type, N> a) noexcept;

    constexpr layout_mapping_regular_base(Dimensions d) noexcept;

    constexpr layout_mapping_regular_base(Dimensions d, Steps s, Pads p) noexcept;

    ///////////////////////////////////////////////////////////////////////////
    // DOMAIN SPACE

    // Order-to-rank mapping. Returns the rank of the dimension with the
    // specified ordering.
    static constexpr size_type otr(size_type order) noexcept;

    // Rank-to-order mapping. Returns the ordering for the dimension with the
    // specified rank.
    static constexpr size_type rto(size_type rank) noexcept;

    static constexpr bool is_regular() noexcept;

    static constexpr bool is_dynamic_stride(size_type rank) noexcept;

    constexpr size_type span() const noexcept;

    constexpr size_type step(size_type rank) const noexcept;
    constexpr Steps steps() const noexcept;

    constexpr size_type pad(size_type rank) const noexcept;
    constexpr Pads pads() const noexcept;

    ///////////////////////////////////////////////////////////////////////////
    // INDEXING

    // Must be implemented by the derived class.
    //constexpr size_type stride(size_type rank) const noexcept;

    // Must be implemented by the derived class.
    //constexpr dimensions<see-below> strides() const noexcept;

    // Must be implemented by the derived class.
    //template <typename... Idxs>
    //constexpr size_type index(Idxs... idxs) const noexcept;

    // Must be implemented by the derived class.
    //template <size_type... IdxDims>
    //constexpr size_type index(dimensions<IdxDims...> i) const noexcept;

    ///////////////////////////////////////////////////////////////////////////

  protected:

    ///////////////////////////////////////////////////////////////////////////

    template <typename Dimensions::size_type Rank>
    static constexpr bool compute_is_dynamic_stride() noexcept;

    // Indexed by order, not rank.
    static constexpr bool computed_is_dynamic_stride_[] =
    {
        compute_is_dynamic_stride<otr(OrderIndices)>()...
    };

    ///////////////////////////////////////////////////////////////////////////

    template <typename Dimensions::size_type Rank, typename Arrayish>
    static constexpr size_type compute_stride(
        Dimensions      d
      , Steps        s
      , Pads         p
      , Arrayish const& last
        ) noexcept;

    template <typename Dimensions::size_type Rank>
    static constexpr size_type compute_static_stride() noexcept;

    // Indexed by order, not rank.
    static constexpr size_type computed_static_stride_[] =
    {
        compute_static_stride<otr(OrderIndices)>()...
    };

    ///////////////////////////////////////////////////////////////////////////

    Steps steps_; // Indexed by order, not rank.
    Pads pads_;   // Indexed by order, not rank.
};

///////////////////////////////////////////////////////////////////////////////
template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base() noexcept
  : base_type()
  , steps_()
  , pads_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
template <typename... DynamicDims>
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base(DynamicDims&&... ddims) noexcept
  : base_type(std::forward<DynamicDims>(ddims)...)
  , steps_()
  , pads_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
template <std::size_t N>
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base(array<value_type, N> a) noexcept
  : base_type(a)
  , steps_()
  , pads_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base(Dimensions d) noexcept
  : base_type(d)
  , steps_()
  , pads_()
{}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::layout_mapping_regular_base(Dimensions d, Steps s, Pads p) noexcept
  : base_type(d)
  , steps_(s)
  , pads_(p) 
{}

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::otr(size_type order) noexcept
{
    return integer_sequence_array<ordering>()[order];
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::rto(size_type rank) noexcept
{
    return integer_sequence_array<inverse_ordering>()[rank];
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr bool layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::is_regular() noexcept
{
    return true; 
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr bool layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::is_dynamic_stride(size_type rank) noexcept
{
    return computed_is_dynamic_stride_[rto(rank)];
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::span() const noexcept
{
    return detail::dims_ternary_reduction<
        detail::span_by_value
      , detail::multiplies_by_value
      , detail::static_sentinel<1>
      , 0
      , Dimensions::rank()
    >()(*static_cast<Dimensions const*>(this), steps_, pads_);
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::step(size_type rank) const noexcept
{
    return steps_[rank];
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr Steps layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::steps() const noexcept
{
    return steps_;
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::pad(size_type rank) const noexcept
{
    return pads_[rank];
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
inline constexpr Pads layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::pads() const noexcept
{
    return pads_;
}

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
// Pass Rank as a template parameter instead of a function parameter to 
// avoid recursion on the same function signature.
template <
    typename Dimensions::size_type Rank
    >
inline constexpr bool layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::compute_is_dynamic_stride() noexcept
{
    // is_dynamic_stride(otr(rto(Rank) - 1)) just looks up the previous
    // entry in the array, so we check it first so that we'll short circuit
    // if it's false.
    return ( is_rank_unit_stride<Rank, Dimensions, ordering>::value
           ? true 
           : is_dynamic_stride(otr(rto(Rank) - 1))
           )
        || Pads::is_dynamic(Rank) || Steps::is_dynamic(Rank);
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr bool layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::computed_is_dynamic_stride_[];

///////////////////////////////////////////////////////////////////////////////

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
// Pass Rank as a template parameter instead of a function parameter to 
// avoid recursion on the same function signature.
template <
    typename Dimensions::size_type Rank
  , typename Arrayish
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::compute_stride(
    Dimensions      d
  , Steps        s
  , Pads         p
  , Arrayish const& last
    ) noexcept
{
    return p[Rank] + s[Rank] * d[otr(rto(Rank) - 1)]
         * ( is_rank_unit_stride<Rank, Dimensions, ordering>::value
           ? size_type(1)
           : last[rto(Rank) - 1] // Look up previously computed value.
           );
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
// Pass Rank as a template parameter instead of a function parameter to 
// avoid recursion on the same function signature.
template <
    typename Dimensions::size_type Rank
    >
inline constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::compute_static_stride() noexcept
{
    return ( is_dynamic_stride(Rank)
           ? dyn
           : compute_stride<Rank>(
                Dimensions(), Steps(), Pads(), computed_static_stride_
             )
           );
}

template <
    typename Dimensions
  , typename Steps
  , typename Pads
  , typename Dimensions::size_type... OrderIndices
    >
constexpr typename layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::size_type layout_mapping_regular_base<
    Dimensions
  , Steps
  , Pads
  , index_sequence<OrderIndices...>
>::computed_static_stride_[];

///////////////////////////////////////////////////////////////////////////////

}}} // std::experimental::detail

#endif // STD_A3B0F119_C38A_44E3_95E4_DD40594F4328

