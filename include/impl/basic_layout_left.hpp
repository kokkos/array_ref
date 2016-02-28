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

    // TYPES 

    using size_type = std::size_t;

    // CONSTRUCTORS

    constexpr basic_layout_left() noexcept;

    constexpr basic_layout_left(
        Striding const& striding
      , Padding const& pad
    ) noexcept;

    // INDEXING 

    template <std::size_t... Dims, typename... Idx>
    size_type
    index(detail::dimensions_impl<Dims...> d, Idx... idx) const noexcept; 

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
        detail::dimensions_impl<Dims...> d
      , IdxHead idx_0, IdxNextHead idx_1, IdxTail... idx_tail
    ) const noexcept;

    // First index, 1 == rank()
    template <std::size_t... Dims
            , typename IdxHead>
    size_type index_impl(
        detail::dimensions_impl<Dims...> d
      , IdxHead idx_0
    ) const noexcept;

    // 0 == rank()
    template <std::size_t... Dims>
    size_type index_impl(
        detail::dimensions_impl<Dims...> d
    ) const noexcept;

    // Nth index
    template <std::size_t N, std::size_t... Dims
            , typename IdxHead, typename IdxNextHead, typename... IdxTail>
    size_type index_impl_n(
        detail::dimensions_impl<Dims...> d
      , IdxHead idx_n, IdxNextHead idx_n_plus_1, IdxTail... idx_tail
    ) const noexcept;

    // Final index
    template <std::size_t N, std::size_t... Dims
            , typename IdxHead>
    size_type index_impl_n(
        detail::dimensions_impl<Dims...> d
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
template <std::size_t... Dims, typename... Idx>
inline typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::index(
    detail::dimensions_impl<Dims...> d, Idx... idx
    ) const noexcept
{
    // TODO: These static asserts need to actually live in multi_array_ref.
    // The first one is particularly important, otherwise it will silently
    // do the wrong thing.
    static_assert(
        detail::dimensions_impl<Dims...>::rank() == sizeof...(idx) 
      , "incorrect number of indices passed to layout");
    static_assert(
        detail::dimensions_impl<Dims...>::rank() == Striding::rank()
      , "dimensions<> passed into layout has wrong rank");
    return index_impl(d, idx...);
}

// First index, 1 < rank()
template <typename Striding, typename Padding>
template <std::size_t... Dims
        , typename IdxHead, typename IdxNextHead, typename... IdxTail>
inline typename basic_layout_left<Striding, Padding>::size_type
basic_layout_left<Striding, Padding>::index_impl(
    detail::dimensions_impl<Dims...> d
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
    detail::dimensions_impl<Dims...> d
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
    detail::dimensions_impl<Dims...> d
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
    detail::dimensions_impl<Dims...> d
  , IdxHead idx_n, IdxNextHead idx_n_plus_1, IdxTail... idx_tail
    ) const noexcept
{
    static_assert(0                                        < N
                , "dimension index is out of bounds in layout");
    static_assert(detail::dimensions_impl<Dims...>::rank() > N
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
    detail::dimensions_impl<Dims...> d
  , IdxHead idx_n
    ) const noexcept
{
    static_assert(0                                        < N
                , "dimension index is out of bounds in layout");
    static_assert(detail::dimensions_impl<Dims...>::rank() > N
                , "dimension index is out of bounds in layout");
    return (d[N-1]*stride_[N-1] + pad_[N-1])
         * (stride_[N]*idx_n);
}

}} // std::experimental

#endif // STD_0F383687_D414_463B_9C79_74EA4545EEB5

