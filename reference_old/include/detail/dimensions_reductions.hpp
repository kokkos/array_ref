///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016 Bryce Adelstein Lelbach aka wash <brycelelbach@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_8703D05A_AE07_4817_90B1_0C0E8607D504)
#define STD_8703D05A_AE07_4817_90B1_0C0E8607D504

#include "detail/fwd.hpp"

namespace std { namespace experimental { namespace detail  {

///////////////////////////////////////////////////////////////////////////////

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Idx
  , std::size_t Size
    >
struct dims_unary_reduction
{
    template <std::size_t... Dims0>
    constexpr typename dimensions<Dims0...>::size_type operator()(
        dimensions<Dims0...> d0
        ) const noexcept
    {
        static_assert(
            std::rank<dimensions<Dims0...> >::value == Size 
          , "Size not equal to rank of argument"
        );
        return Reduction()(
            Op()(d0[Idx])
          , dims_unary_reduction<Op, Reduction, Sentinel, Idx + 1, Size>()
                (std::move(d0))
        );
    }
};

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Size
    >
struct dims_unary_reduction<Op, Reduction, Sentinel, Size, Size>
{
    template <std::size_t... Dims0>
    constexpr typename dimensions<Dims0...>::size_type operator()(
        dimensions<Dims0...> d0
        ) const noexcept
    {
        return Sentinel()();
    }
};

///////////////////////////////////////////////////////////////////////////////

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Idx
  , std::size_t Size
    >
struct dims_binary_reduction
{
    template <std::size_t... Dims0, std::size_t... Dims1>
    constexpr typename dimensions<Dims0...>::size_type operator()(
        dimensions<Dims0...> d0
      , dimensions<Dims1...> d1
        ) const noexcept
    {
        static_assert(
            dimensions<Dims0...>::rank() == dimensions<Dims1...>::rank()
          , "Arguments to binary reduction have unequal rank"
        );
        static_assert(
               dimensions<Dims0...>::rank() == Size 
            || dimensions<Dims1...>::rank() == Size 
          , "Size not equal to rank of arguments"
        );

        return Reduction()(
            Op()(d0[Idx], d1[Idx])
          , dims_binary_reduction<Op, Reduction, Sentinel, Idx + 1, Size>()
                (std::move(d0), std::move(d1))
        );
    }
};

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Size
    >
struct dims_binary_reduction<Op, Reduction, Sentinel, Size, Size>
{
    template <std::size_t... Dims0, std::size_t... Dims1>
    constexpr typename dimensions<Dims0...>::size_type operator()(
        dimensions<Dims0...> d0
      , dimensions<Dims1...> d1
        ) const noexcept
    {
        static_assert(
            dimensions<Dims0...>::rank() == dimensions<Dims1...>::rank()
          , "Arguments to binary reduction have unequal rank"
        );
        static_assert(
               dimensions<Dims0...>::rank() == Size 
            || dimensions<Dims1...>::rank() == Size 
          , "Size not equal to rank of arguments"
        );

        return Sentinel()();
    }
};

///////////////////////////////////////////////////////////////////////////////

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Idx
  , std::size_t Size
    >
struct dims_ternary_reduction
{ 
    template <std::size_t... Dims0, std::size_t... Dims1, std::size_t... Dims2>
    constexpr typename dimensions<Dims0...>::size_type operator()(
        dimensions<Dims0...> d0
      , dimensions<Dims1...> d1
      , dimensions<Dims2...> d2
        ) const noexcept
    {
        static_assert(
               dimensions<Dims0...>::rank() == dimensions<Dims1...>::rank()
            && dimensions<Dims0...>::rank() == dimensions<Dims2...>::rank()
          , "Arguments to ternary reduction have unequal rank"
        );
        static_assert(
               dimensions<Dims0...>::rank() == Size 
            || dimensions<Dims1...>::rank() == Size 
            || dimensions<Dims2...>::rank() == Size 
          , "Size not equal to rank of arguments"
        );

        return Reduction()(
            Op()(d0[Idx], d1[Idx], d2[Idx])
          , dims_ternary_reduction<Op, Reduction, Sentinel, Idx + 1, Size>()
                (std::move(d0), std::move(d1), std::move(d2))
        );
    }
};

template <
    typename Op
  , typename Reduction
  , typename Sentinel
  , std::size_t Size
    >
struct dims_ternary_reduction<Op, Reduction, Sentinel, Size, Size>
{
    template <std::size_t... Dims0, std::size_t... Dims1, std::size_t... Dims2>
    constexpr typename dimensions<Dims0...>::size_type operator()(
        dimensions<Dims0...> d0
      , dimensions<Dims1...> d1
      , dimensions<Dims2...> d2
        ) const noexcept
    {
        static_assert(
               dimensions<Dims0...>::rank() == dimensions<Dims1...>::rank()
            && dimensions<Dims0...>::rank() == dimensions<Dims2...>::rank()
          , "Arguments to ternary reduction have unequal rank"
        );
        static_assert(
               dimensions<Dims0...>::rank() == Size 
            || dimensions<Dims1...>::rank() == Size 
            || dimensions<Dims2...>::rank() == Size 
          , "Size not equal to rank of arguments"
        );

        return Sentinel()();
    }
};

///////////////////////////////////////////////////////////////////////////////

struct identity_by_value
{
    template <typename T>
    constexpr T operator()(T v0) const noexcept
    {
        return std::move(v0);
    }
};

struct multiplies_by_value
{
    template <typename T>
    constexpr T operator()(T v0, T v1) const noexcept
    {
        return v0 * v1;
    }
};

struct span_by_value
{
    template <typename T>
    constexpr T operator()(T d, T s, T p) const noexcept
    {
        return (d * s) + p;
    }
};

template <std::size_t Value> 
struct static_sentinel
{
    constexpr std::size_t operator()() const noexcept
    {
        return Value;
    }
};

}}} // std::experimental::detail

#endif // STD_8703D05A_AE07_4817_90B1_0C0E8607D504

