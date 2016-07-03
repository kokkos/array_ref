///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_836A92EF_4885_4F3B_A7E1_5395FEDD3125)
#define STD_836A92EF_4885_4F3B_A7E1_5395FEDD3125

#include "impl/fwd.hpp"
#include "impl/dimensions.hpp"

namespace std { namespace experimental 
{

template <typename Striding, typename Padding>
struct basic_layout_right
{
    static_assert(Striding::rank() == Padding::rank() 
                , "striding and padding have different rank");

    ///////////////////////////////////////////////////////////////////////////
    // TYPES

    using size_type = std::size_t;

    /////////////////////////////////////////////////////////////////////////// 
    // CONSTRUCTORS AND ASSIGNMENT OPERATORS

    constexpr basic_layout_right() noexcept;

    constexpr basic_layout_right(basic_layout_right const& b) noexcept = default;
    constexpr basic_layout_right(basic_layout_right&& b) noexcept = default;
    basic_layout_right& operator=(basic_layout_right const& b) noexcept = default;
    basic_layout_right& operator=(basic_layout_right&& b) noexcept = default;

    constexpr basic_layout_right(
        Striding const& striding, Padding const& pad
        ) noexcept;

    /////////////////////////////////////////////////////////////////////////// 
    // DOMAIN SPACE 

    static constexpr bool is_regular() noexcept;    

    constexpr size_type stride(size_type rank) noexcept;

    template <std::size_t... Dims>
    constexpr size_type span(dimensions<Dims...> d) const noexcept;    

    /////////////////////////////////////////////////////////////////////////// 
    // INDEXING 

    template <std::size_t... Dims, typename... Idx>
    size_type index(dimensions<Dims...> d, Idx... idx) const noexcept; 

  private:
    Striding stride_; 
    Padding pad_; 
};  

template <typename Striding, typename Padding>
constexpr
basic_layout_right<Striding, Padding>::basic_layout_right()
    noexcept
  : stride_()
  , pad_()
{}

template <typename Striding, typename Padding>
constexpr
basic_layout_right<Striding, Padding>::basic_layout_right(
    Striding const& striding
  , Padding const& pad
    ) noexcept
  : stride_(striding)
  , pad_(pad)
{}

template <typename Striding, typename Padding>
inline constexpr bool
basic_layout_right<Striding, Padding>::is_regular() noexcept
{
    return true;
}

template <typename Striding, typename Padding>
inline constexpr typename basic_layout_right<Striding, Padding>::size_type
basic_layout_right<Striding, Padding>::stride(size_type rank) const noexcept
{
    return stride_[rank];
}

template <typename Striding, typename Padding>
template <std::size_t... Dims>
inline constexpr typename basic_layout_right<Striding, Padding>::size_type
basic_layout_right<Striding, Padding>::span(dimensions<Dims...> d) const noexcept
{
    return detail::dims_ternary_reduction<
        detail::span_by_value
      , detail::multiplies_by_value
      , detail::static_sentinel<1>
      , 0
      , dimensions<Dims...>::rank()
    >()(d, stride_, pad_);
}

template <typename Striding, typename Padding>
template <std::size_t... Dims, typename... Idx>
inline typename basic_layout_right<Striding, Padding>::size_type
basic_layout_right<Striding, Padding>::index(
    dimensions<Dims...> d, Idx... idx
    ) const noexcept
{
    // TODO: These static asserts need to actually live in array_ref. The
    // first one is particularly important, otherwise it will silently do the
    // wrong thing.
    static_assert(
        dimensions<Dims...>::rank() == sizeof...(idx) 
      , "incorrect number of indices passed to basic_layout_right");
    static_assert(
        dimensions<Dims...>::rank() == Striding::rank()
      , "dimensions<> passed into basic_layout_right has wrong rank");

    using dims = dimensions<Dims...>;

    detail::basic_layout_right_indexer<dims, dims::rank() - 1> indexer;
    auto i = detail::make_filled_dims_t<dims::rank(), dyn>(idx...); 

    return indexer(stride_, pad_, d, i);
}

///////////////////////////////////////////////////////////////////////////////

namespace detail
{

// Recursive row-major layout implementation.
//
// Three initial cases:
// * First index, 1 < rank()
// * First index, 1 == rank()
// * 0 == rank()
//
// The first case (1 < rank()) recurses, with cases: 
// * Nth index
// * Final index

// Nth index 
template <typename Dimensions, std::size_t N, typename enable>
struct basic_layout_right_indexer
{
    template <std::size_t... IdxDims, typename Striding, typename Padding>
    typename basic_layout_right<Striding, Padding>::size_type
    operator()(
        Striding stride
      , Padding pad
      , Dimensions d
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        static_assert(0                  < N
                    , "dimension index is out of bounds in layout_right");
        static_assert(Dimensions::rank() > N
                    , "dimension index is out of bounds in layout_right");
        basic_layout_right_indexer<Dimensions, N + 1> const next_indexer;
        auto constexpr R = Dimensions::rank();
        return (d[R-N]*stride[R-N] + pad[R-N])
             * ( stride[(R-1)-N]*i[(R-1)-N]
               + next_indexer(stride, pad, d, i)
               );
    }
};

// First index, 1 < rank()
template <typename Dimensions>
struct basic_layout_right_indexer<
    Dimensions
  , 0                                                // First index
  , typename enable_if<1 < Dimensions::rank()>::type // 1 < rank()
>
{
    template <std::size_t... IdxDims, typename Striding, typename Padding>
    typename basic_layout_right<Striding, Padding>::size_type
    operator()(
        Striding stride
      , Padding pad
      , Dimensions d
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        basic_layout_right_indexer<Dimensions, 1> const next_indexer;
        auto constexpr R = Dimensions::rank();
        return stride[R-1]*i[R-1]
             + next_indexer(stride, pad, d, i);
    }
};

// First index, 1 == rank()
template <typename Dimensions>
struct basic_layout_right_indexer<
    Dimensions
  , 0                                                 // First index
  , typename enable_if<1 == Dimensions::rank()>::type // 1 == rank()
>
{
    template <std::size_t... IdxDims, typename Striding, typename Padding>
    typename basic_layout_right<Striding, Padding>::size_type
    operator()(
        Striding stride
      , Padding pad
      , Dimensions d
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        auto constexpr R = Dimensions::rank();
        return stride[R-1]*i[R-1];
    }
};

// 0 == rank() 
template <typename Dimensions, std::size_t N>
struct basic_layout_right_indexer<
    Dimensions
  , N
  , typename enable_if<0 == Dimensions::rank()>::type // 0 == rank()
>
{
    template <std::size_t... IdxDims, typename Striding, typename Padding>
    typename basic_layout_right<Striding, Padding>::size_type
    operator()(
        Striding stride
      , Padding pad
      , Dimensions d
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        return 0;
    }
};

// Final index
template <typename Dimensions, std::size_t N>
struct basic_layout_right_indexer<
    Dimensions
  , N
  , typename enable_if<
        (1 < Dimensions::rank())
     && (N == (Dimensions::rank() - 1)) // Final index
    >::type 
>
{
    template <std::size_t... IdxDims, typename Striding, typename Padding>
    typename basic_layout_right<Striding, Padding>::size_type
    operator()(
        Striding stride
      , Padding pad
      , Dimensions d
      , dimensions<IdxDims...> i
        ) const noexcept
    {
        static_assert(0                  < N
                    , "dimension index is out of bounds in layout_right");
        static_assert(Dimensions::rank() > N
                    , "dimension index is out of bounds in layout_right");
        auto constexpr R = Dimensions::rank();
        return (d[R-N]*stride[R-N] + pad[R-N]) * (stride[(R-1)-N]*i[(R-1)-N]);
    }
};

}}} // std::experimental::detail

#endif // STD_836A92EF_4885_4F3B_A7E1_5395FEDD3125

