///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_F2BEAFA8_F13E_481C_8A36_200DCD59D101)
#define STD_F2BEAFA8_F13E_481C_8A36_200DCD59D101

#include <type_traits>

namespace std { namespace experimental
{

template<typename...>
struct conjunction;

template<>
struct conjunction<> : std::true_type {};

template <typename T>
struct conjunction<T> : T {};

template <typename T1, typename T2>
struct conjunction<T1, T2> : std::conditional<T1::value, T2, T1>::type {};

template<typename T1, typename T2, typename T3, typename... TN>
struct conjunction<T1, T2, T3, TN...>
  : std::conditional<T1::value, conjunction<T2, T3, TN...>, T1>::type
{};

}}

#endif // STD_F2BEAFA8_F13E_481C_8A36_200DCD59D101
