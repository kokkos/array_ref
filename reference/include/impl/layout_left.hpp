///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E)
#define STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E

#include "impl/fwd.hpp"
#include "impl/basic_layout_left.hpp"

namespace std { namespace experimental
{

struct layout_left
{
    // TYPES 

    using size_type =
        typename basic_layout_left<dimensions<>, dimensions<> >::size_type;

    // CONSTRUCTORS

    constexpr layout_left() noexcept = default;

    // INDEXING 

    template <std::size_t... Dims, typename... Idx>
    size_type index(dimensions<Dims...> d, Idx... idx) const noexcept; 
};


template <std::size_t... Dims, typename... Idx>
inline layout_left::size_type
layout_left::index(dimensions<Dims...> d, Idx... idx) const noexcept
{
    // basic_layout_left will static_assert when passed an incorrect number
    // of indices. 
    using striding = detail::make_filled_dims_t<sizeof...(Dims), 1>;
    using padding  = detail::make_filled_dims_t<sizeof...(Dims), 0>;
    using layout = basic_layout_left<striding, padding>;
    return layout().index(d, idx...);
}

}} // std::experimental

#endif // STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E

