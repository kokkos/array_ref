///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_AF6B6020_7733_4741_942B_D95071B4FB7B)
#define STD_AF6B6020_7733_4741_942B_D95071B4FB7B

#include "impl/fwd.hpp"
#include "impl/meta.hpp"

namespace std { namespace experimental 
{

template <std::size_t... Dims>
struct dimensions
{
    // TYPES

    // NOTE: We may need/want the full set of container types.

    using value_type = std::size_t;
    using size_type = std::size_t;

    // CONSTRUCTORS, DESTRUCTORS, ASSIGNMENT OPERATORS

    ~dimensions() = default;

    // Default constructor. Default-initializes any dynamic dimensions.
    constexpr dimensions() noexcept;

    // Constructs from a set of dynamic dimensions.
    // EXPECTS: rank_dynamic() == sizeof...(DynamicDims)
    // EXPECTS: std::is_integral<> is true for all the types in Sizes
    template <typename... DynamicDims>
    constexpr dimensions(DynamicDims... ddims) noexcept;

    // Copy constructor.
    constexpr dimensions(dimensions const&) = default;

    // Move constructor.
    constexpr dimensions(dimensions&&) = default;

    // Copy assignment operator.
    dimensions& operator=(dimensions const&) = default;

    // Move assignment operator.
    dimensions& operator=(dimensions&&) = default;

    // METADATA ACCESS

    // Returns the number of dimensions of the referenced array.
    static constexpr size_type rank() noexcept;

    // Returns the number of dimension which are dynamic.
    // NOTE: Not currently in spec.
    static constexpr size_type rank_dynamic() noexcept;

    constexpr size_type size() noexcept;

    // Member accessor.
    // NOTE: Spec needs to clarify the return value of this function if idx
    // is out of bound. Currently, you get 0.
    template <typename IntegralType>
    constexpr value_type operator[](IntegralType idx) const noexcept;

  private:
    ///////////////////////////////////////////////////////////////////////////

    // Computes the product of all extents. Pass 0 as idx and unpack Dims when
    // calling. 

    // Base case.
    template <typename Idx>
    constexpr size_type product_extents(
        Idx idx
        ) noexcept;

    template <typename Idx, typename Head, typename... Tail>
    constexpr size_type product_extents(
        Idx idx, Head head, Tail... tail
        ) noexcept;

    detail::make_dynamic_dims_array_t<Dims...> dynamic_dims_;
};

// FIXME: Confirm that default-initializing an integral type is guranteed to
// zero it. If it's not, maybe do an inlined memset (don't actually call
// memset, because lol icpc).
template <std::size_t... Dims>
constexpr
dimensions<Dims...>::dimensions() noexcept
  : dynamic_dims_{} {}

template <std::size_t... Dims>
template <typename... DynamicDims>
constexpr
dimensions<Dims...>::dimensions(DynamicDims... ddims) noexcept
  // FIXME: We cast here to avoid a narrowing conversion warning from GCC.
  // I'm not thrilled about it.
  : dynamic_dims_{static_cast<value_type>(ddims)...}
{
    static_assert(
        detail::pack_is_integral<DynamicDims...>::value
      , "Non-integral types passed to dimensions<> constructor" 
    );
    static_assert(
        detail::count_dynamic_dims<Dims...>::value == sizeof...(DynamicDims)
      , "Incorrect number of dynamic dimensions passed to dimensions<>"
        );
}

template <std::size_t... Dims>
inline constexpr typename dimensions<Dims...>::size_type
dimensions<Dims...>::rank() noexcept
{
    return std::rank<dimensions>::value;
}

template <std::size_t... Dims>
inline constexpr typename dimensions<Dims...>::size_type
dimensions<Dims...>::rank_dynamic() noexcept
{
    return detail::count_dynamic_dims<Dims...>::value;
}

template <std::size_t... Dims>
inline constexpr typename dimensions<Dims...>::size_type
dimensions<Dims...>::size() noexcept
{
    return product_extents(0, Dims...);
}

template <std::size_t... Dims>
template <typename IntegralType>
inline constexpr typename dimensions<Dims...>::value_type
dimensions<Dims...>::operator[](IntegralType idx) const noexcept
{
    return ( detail::dynamic_extent(idx, Dims...) == dyn
           ? dynamic_dims_[detail::index_into_dynamic_dims(idx, Dims...)]
           : detail::dynamic_extent(idx, Dims...)
           );
} 

// Base case.
template <std::size_t... Dims>
template <typename Idx>
inline constexpr typename dimensions<Dims...>::size_type
dimensions<Dims...>::product_extents(Idx idx) noexcept
{
    return 1;
}

template <std::size_t... Dims>
template <typename Idx, typename Head, typename... Tail>
inline constexpr typename dimensions<Dims...>::size_type
dimensions<Dims...>::product_extents(Idx idx, Head head, Tail... tail) noexcept
{
    return (head == dyn ? (*this)[idx] : head)
         * product_extents(idx + 1, tail...);
}

///////////////////////////////////////////////////////////////////////////////

namespace detail
{

// Base case.
template <typename Idx>
inline constexpr std::size_t dynamic_extent(
    Idx idx
    ) noexcept
{
    return 0; 
}

template <typename Idx, typename Head, typename... Tail>
inline constexpr std::size_t dynamic_extent(
    Idx idx, Head head, Tail... tail
    ) noexcept
{
    return ( idx == 0
           ? head
           : dynamic_extent(idx - 1, tail...)
           );        
}

// Base case.
template <typename Idx>
inline constexpr std::size_t index_into_dynamic_dims(
    Idx idx
    ) noexcept
{
    return 0; 
}

template <typename Idx, typename Head, typename... Tail>
inline constexpr std::size_t index_into_dynamic_dims(
    Idx idx, Head head, Tail... tail
    ) noexcept
{
    return
        // FIXME: Is idx != 0 needed
        ( head == dyn && idx != 0
        ? index_into_dynamic_dims((idx != 0 ? idx - 1 : idx), tail...) + 1
        : index_into_dynamic_dims((idx != 0 ? idx - 1 : idx), tail...)
        );
}

}}} // std::experimental::detail

#endif // STD_AF6B6020_7733_4741_942B_D95071B4FB7B

