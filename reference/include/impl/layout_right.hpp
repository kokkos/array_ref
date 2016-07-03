///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_68717262_A4A1_41F8_94D6_FF64D30C3903)
#define STD_68717262_A4A1_41F8_94D6_FF64D30C3903

#include "impl/fwd.hpp"
#include "impl/basic_layout_right.hpp"

namespace std { namespace experimental
{

struct layout_right
{
    // TYPES 

    using size_type =
        typename basic_layout_right<dimensions<>, dimensions<> >::size_type;

    // CONSTRUCTORS

    constexpr layout_right() noexcept = default;

    // INDEXING 

    template <std::size_t... Dims, typename... Idx>
    size_type index(dimensions<Dims...> d, Idx... idx) const noexcept; 
};


template <std::size_t... Dims, typename... Idx>
inline layout_right::size_type
layout_right::index(dimensions<Dims...> d, Idx... idx) const noexcept
{
    // basic_layout_right will static_assert when passed an incorrect number
    // of indices. 
    using striding = detail::make_filled_dims_t<sizeof...(Dims), 1>;
    using padding  = detail::make_filled_dims_t<sizeof...(Dims), 0>;
    using layout = basic_layout_right<striding, padding>;
    return layout().index(d, idx...);
}

}} // std::experimental

#endif // STD_68717262_A4A1_41F8_94D6_FF64D30C3903

