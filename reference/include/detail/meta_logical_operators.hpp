///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_F2BEAFA8_F13E_481C_8A36_200DCD59D101)
#define STD_F2BEAFA8_F13E_481C_8A36_200DCD59D101

//#warning Unit test coverage needed

#include <type_traits>

namespace std { namespace experimental { namespace detail {

///////////////////////////////////////////////////////////////////////////////

template <typename... Ts>
struct conjunction;

template <>
struct conjunction<> : true_type {};

template <typename T>
struct conjunction<T> : T {};

template <typename T0, typename T1>
struct conjunction<T0, T1> : conditional<T0::value, T1, T0>::type {};

template<typename T0, typename T1, typename T2, typename... TN>
struct conjunction<T0, T1, T2, TN...>
  : conditional<T0::value, conjunction<T1, T2, TN...>, T0>::type {};

///////////////////////////////////////////////////////////////////////////////

template <typename... Ts>
struct disjunction;

template <typename... Ts>
struct disjunction : false_type {};

template <typename T>
struct disjunction<T> : T {};

template <typename T0, typename... TN>
struct disjunction<T0, TN...>
  : conditional<T0::value != false, T0, disjunction<TN...> >::type {};

///////////////////////////////////////////////////////////////////////////////

template <typename T>
struct negation;

template <typename T>
struct negation : std::integral_constant<bool, !T::value> {};

}}} // std::experimental::detail

#endif // STD_F2BEAFA8_F13E_481C_8A36_200DCD59D101

