///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_0F383687_D414_463B_9C79_74EA4545EEB5)
#define STD_0F383687_D414_463B_9C79_74EA4545EEB5

#include "impl/fwd.hpp"
#include "impl/dimensions.hpp"

namespace std { namespace experimental
{

template <typename Striding, typename Padding>
struct basic_layout_left
{
    static_assert(Striding::rank() == Padding::rank() 
                , "striding and padding have different rank");

    ///////////////////////////////////////////////////////////////////////////
    // TYPES

    using size_type = std::size_t;

    /////////////////////////////////////////////////////////////////////////// 
    // CONSTRUCTORS AND ASSIGNMENT OPERATORS

    constexpr basic_layout_left() noexcept;

    constexpr basic_layout_left(basic_layout_left const& b) noexcept = default;
    constexpr basic_layout_left(basic_layout_left&& b) noexcept = default;
    basic_layout_left& operator=(basic_layout_left const& b) noexcept = default;
    basic_layout_left& operator=(basic_layout_left&& b) noexcept = default;

    constexpr basic_layout_left(
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
    // Recursive column-major layout implementation.
    //
    // Three initial cases:
    // * First index, 1 < rank()
    // * First index, 1 == rank()
    // * 0 == rank()
    //
    // The first case (1 < rank()) recurses, with cases: 
    // * Nth index
    // * Final index

    // First index, 1 < rank()
    template <std::size_t... Dims
            , typename IdxHead, typename IdxNextHead, typename... IdxTail>
    size_type index_impl(
        dimensions<Dims...> d
      , IdxHead idx_0, IdxNextHead idx_1, IdxTail... idx_tail
    ) const noexcept;

    // First index, 1 == rank()
    template <std::size_t... Dims
            , typename IdxHead>
    size_type index_impl(
        dimensions<Dims...> d
      , IdxHead idx_0
    ) const noexcept;

    // 0 == rank()
    template <std::size_t... Dims>
    size_type index_impl(
        dimensions<Dims...> d
    ) const noexcept;

    // Nth index
    template <std::size_t N, std::size_t... Dims
            , typename IdxHead, typename IdxNextHead, typename... IdxTail>
    size_type index_impl_n(
        dimensions<Dims...> d
      , IdxHead idx_n, IdxNextHead idx_n_plus_1, IdxTail... idx_tail
    ) const noexcept;

    // Final index
    template <std::size_t N, std::size_t... Dims
            , typename IdxHead>
    size_type index_impl_n(
        dimensions<Dims...> d
      , IdxHead idx_n
    ) const noexcept;

    Striding stride_; 
    Padding pad_; 
};  

template <typename Striding, typename Padding>
constexpr
basic_layout_left<Striding, Padding>::basic_layout_left()
    noexcept
  : stride_()
  , pad_()
{}

template <typename Striding, typename Padding>
constexpr
basic_layout_left<Striding, Padding>::basic_layout_left(
    Striding const& striding
  , Padding const& pad
    ) noexcept
  : stride_(striding)
  , pad_(pad)
{}

template <typename Striding, typename Padding>
inline constexpr bool
basic_layout_left<Striding, Padding>::is_regular() noexcept
{
    return true;
}

template <typename Striding, typename Padding>
inline constexpr typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::stride(size_type rank) const noexcept
{
    return stride_[rank];
}

template <typename Striding, typename Padding>
template <std::size_t... Dims>
inline constexpr typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::span(dimensions<Dims...> d) const noexcept
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
inline typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::index(
    dimensions<Dims...> d, Idx... idx
    ) const noexcept
{
    // TODO: These static asserts need to actually live in array_ref. The
    // first one is particularly important, otherwise it will silently do the
    // wrong thing.
    static_assert(
        dimensions<Dims...>::rank() == sizeof...(idx) 
      , "incorrect number of indices passed to basic_layout_left");
    static_assert(
        dimensions<Dims...>::rank() == Striding::rank()
      , "dimensions<> passed into basic_layout_left has wrong rank");
    return index_impl(d, idx...);
}

// First index, 1 < rank()
template <typename Striding, typename Padding>
template <std::size_t... Dims
        , typename IdxHead, typename IdxNextHead, typename... IdxTail>
inline typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::index_impl(
    dimensions<Dims...> d
  , IdxHead idx_0, IdxNextHead idx_1, IdxTail... idx_tail
    ) const noexcept
{
    return stride_[0]*idx_0
         + index_impl_n<1>(d, idx_1, idx_tail...);
}

// First index, 1 == rank()
template <typename Striding, typename Padding>
template <std::size_t... Dims
        , typename IdxHead>
inline typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::index_impl(
    dimensions<Dims...> d
  , IdxHead idx_0
    ) const noexcept
{
    return stride_[0]*idx_0;
}

// 0 == rank() 
template <typename Striding, typename Padding>
template <std::size_t... Dims>
inline typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::index_impl(
    dimensions<Dims...> d
    ) const noexcept
{
    return 0;
}

// Nth index
template <typename Striding, typename Padding>
template <std::size_t N, std::size_t... Dims
        , typename IdxHead, typename IdxNextHead, typename... IdxTail>
inline typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::index_impl_n(
    dimensions<Dims...> d
  , IdxHead idx_n, IdxNextHead idx_n_plus_1, IdxTail... idx_tail
    ) const noexcept
{
    static_assert(0                           < N
                , "dimension index is out of bounds in layout");
    static_assert(dimensions<Dims...>::rank() > N
                , "dimension index is out of bounds in layout");
    return (d[N-1]*stride_[N-1] + pad_[N-1])
         * ( stride_[N]*idx_n
           + index_impl_n<N+1>(d, idx_n_plus_1, idx_tail...));
}

// Final index
template <typename Striding, typename Padding>
template <std::size_t N, std::size_t... Dims
        , typename IdxHead>
inline typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::index_impl_n(
    dimensions<Dims...> d
  , IdxHead idx_n
    ) const noexcept
{
    static_assert(0                           < N
                , "dimension index is out of bounds in layout");
    static_assert(dimensions<Dims...>::rank() > N
                , "dimension index is out of bounds in layout");
    return (d[N-1]*stride_[N-1] + pad_[N-1])
         * (stride_[N]*idx_n);
}

}} // std::experimental

#endif // STD_0F383687_D414_463B_9C79_74EA4545EEB5

