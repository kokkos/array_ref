////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>
#include <iostream>

template <typename T, std::size_t N>
inline std::ostream& operator<<(std::ostream& os, std::array<T, N> const& a)
{
    os << "{ ";
    for (auto const& e: a) os << e << " ";
    os << "}";
    return os;
}

using std::array;
using std::experimental::dyn;
using std::experimental::detail::filter_initialize_dynamic_dims_array;

template <std::size_t N>
using sz_array = array<std::size_t, N>;

int main()
{
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        (0, sz_array<0>{{ }}))
      , (sz_array<0>{{ }})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        (0, sz_array<1>{{0}}))
      , (sz_array<1>{{0}})
    );

    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <  3>(0, sz_array<0>{{ }}, 3))
      , (sz_array<0>{{ }})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <dyn>(0, sz_array<1>{{0}}, 3))
      , (sz_array<1>{{3}})
    );

    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <  3,   5>(0, sz_array<0>{{ }}, 1, 2))
      , (sz_array<0>{{ }})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <dyn,   5>(0, sz_array<1>{{0}}, 3, 5))
      , (sz_array<1>{{3}})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <  3, dyn>(0, sz_array<1>{{0}}, 3, 5))
      , (sz_array<1>{{5}})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <dyn, dyn>(0, sz_array<2>{{0, 0}}, 3, 5))
      , (sz_array<2>{{3, 5}})
    );

    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <  3,   5,   9>(0, sz_array<0>{{ }}, 3, 5, 9))
      , (sz_array<0>{{ }})
    );

    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <dyn,   5,   9>(0, sz_array<1>{{0}}, 3, 5, 9))
      , (sz_array<1>{{3}})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <  3, dyn,   9>(0, sz_array<1>{{0}}, 3, 5, 9))
      , (sz_array<1>{{5}})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <  3,   5, dyn>(0, sz_array<1>{{0}}, 3, 5, 9))
      , (sz_array<1>{{9}})
    );

    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <  3, dyn, dyn>(0, sz_array<2>{{0, 0}}, 3, 5, 9))
      , (sz_array<2>{{5, 9}})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <dyn,   5, dyn>(0, sz_array<2>{{0, 0}}, 3, 5, 9))
      , (sz_array<2>{{3, 9}})
    );
    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <dyn, dyn,   9>(0, sz_array<2>{{0, 0}}, 3, 5, 9))
      , (sz_array<2>{{3, 5}})
    );

    BOOST_TEST_EQ((filter_initialize_dynamic_dims_array
        <dyn, dyn, dyn>(0, sz_array<3>{{0, 0, 0}}, 3, 5, 9))
      , (sz_array<3>{{3, 5, 9}})
    );

    return boost::report_errors();
}

