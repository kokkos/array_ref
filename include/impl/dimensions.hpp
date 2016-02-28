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

namespace std { namespace experimental { namespace detail
{

template <std::size_t... Dims>
struct dimensions_impl
{
    // TYPES

    // NOTE: We may need/want the full set of container types.

    using value_type = std::size_t;
    using size_type = std::size_t;

    // NOTE: Not defined in the spec, public for unit tests. 
    using dynamic_dims_type = typename detail::build_dims_array<Dims...>::type;

    // CONSTRUCTORS, DESTRUCTORS, ASSIGNMENT OPERATORS

    ~dimensions_impl() = default;

    // Default constructor. Default-initializes any dynamic dimensions.
    constexpr dimensions_impl() noexcept;

    // Constructs from a set of dynamic dimensions.
    // EXPECTS: num_dynamic_dims() == sizeof...(DynamicDims)
    // EXPECTS: std::is_integral<> is true for all the types in Sizes
    template <typename... DynamicDims>
    constexpr dimensions_impl(DynamicDims... ddims) noexcept;

    // Copy constructor.
    constexpr dimensions_impl(dimensions_impl const&) = default;

    // Move constructor.
    constexpr dimensions_impl(dimensions_impl&&) = default;

    // Copy assignment operator.
    dimensions_impl& operator=(dimensions_impl const&) = default;

    // Move assignment operator.
    dimensions_impl& operator=(dimensions_impl&&) = default;

    // METADATA ACCESS

    // Returns the number of dimensions of the referenced array.
    static constexpr value_type rank() noexcept;

    // Returns the number of dimension which are dynamic.
    // NOTE: Not currently in spec.
    static constexpr value_type num_dynamic_dims() noexcept;

    // Member accessor.
    // NOTE: Spec needs to clarify the return value of this function if idx
    // is out of bound. Currently, you get 0.
    template <typename IntegralType>
    constexpr value_type operator[](IntegralType idx) const noexcept;

  private:
    dynamic_dims_type dynamic_dims_;
};

// FIXME: Confirm that default-initializing an integral type is guranteed to
// zero it. If it's not, maybe do an inlined memset (don't actually call
// memset, because lol icpc).
template <std::size_t... Dims>
constexpr
dimensions_impl<Dims...>::dimensions_impl() noexcept
  : dynamic_dims_{} {}

template <std::size_t... Dims>
template <typename... DynamicDims>
constexpr
dimensions_impl<Dims...>::dimensions_impl(DynamicDims... ddims) noexcept
  // FIXME: We cast here to avoid a narrowing conversion warning from GCC.
  // I'm not thrilled about it.
  : dynamic_dims_{static_cast<value_type>(ddims)...}
{
    static_assert(
        detail::pack_is_integral<DynamicDims...>::value
      , "Non-integral types passed to dimensions<> constructor" 
    );
    static_assert(
        detail::count_dynamic_dims<Dims...>::value
        == sizeof...(DynamicDims)
      , "Incorrect number of dynamic dimensions passed to dimensions<>"
        );
}

template <std::size_t... Dims>
inline constexpr typename dimensions_impl<Dims...>::value_type
dimensions_impl<Dims...>::rank() noexcept
{
    return std::rank<dimensions_impl>::value;
}

template <std::size_t... Dims>
inline constexpr typename dimensions_impl<Dims...>::value_type
dimensions_impl<Dims...>::num_dynamic_dims() noexcept
{
    return detail::count_dynamic_dims<Dims...>::value;
}

template <std::size_t... Dims>
template <typename IntegralType>
inline constexpr typename dimensions_impl<Dims...>::value_type
dimensions_impl<Dims...>::operator[](IntegralType idx) const noexcept
{
    return ( dynamic_extent(idx, Dims...) == dynamic_dimension
           ? dynamic_dims_[index_into_dynamic_dims(idx, Dims...)]
           : dynamic_extent(idx, Dims...)
           );
} 

///////////////////////////////////////////////////////////////////////////////

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
        ( head == dynamic_dimension && idx != 0
        ? index_into_dynamic_dims((idx != 0 ? idx - 1 : idx), tail...) + 1
        : index_into_dynamic_dims((idx != 0 ? idx - 1 : idx), tail...)
        );
}

}}} // std::experimental::detail

#endif // STD_AF6B6020_7733_4741_942B_D95071B4FB7B

