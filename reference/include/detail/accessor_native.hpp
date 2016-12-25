///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_DD4E6A35_2A89_4627_A2FF_1C123282BC98)
#define STD_DD4E6A35_2A89_4627_A2FF_1C123282BC98

#include <type_traits>

namespace std { namespace experimental  {

template <typename ValueType>
struct accessor_native
{
    // TYPES

    using value_type = ValueType; 
    using pointer    = typename add_pointer<value_type>::type;
    using reference  = typename add_lvalue_reference<value_type>::type;

    // CONSTRUCTORS

    constexpr accessor_native() noexcept = default;

    // ACCESS 

    template <typename IntegralType>
    reference access(pointer p, IntegralType idx) const noexcept; 
};

template <typename ValueType>
template <typename IntegralType>
inline typename accessor_native<ValueType>::reference
accessor_native<ValueType>::access(pointer p, IntegralType idx) const noexcept
{
    return p[idx];
}

}} // std::experimental

#endif // STD_DD4E6A35_2A89_4627_A2FF_1C123282BC98

