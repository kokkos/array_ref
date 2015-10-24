////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include "space.hpp"

int main()
{
    using boost::detail::dynamic_extent_tuple_index;
    constexpr auto ext = boost::dynamic_extent;

    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0         >::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, 3      >::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, 3, 5   >::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, 3, 5, 9>::value), 0);

    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, ext, 5,   9  >::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<1, 0, ext, 5,   9  >::value), 1);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<2, 0, ext, 5,   9  >::value), 1);

    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, 3,   ext, 9  >::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<1, 0, 3,   ext, 9  >::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<2, 0, 3,   ext, 9  >::value), 1);

    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, 3,   5,   ext>::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<1, 0, 3,   5,   ext>::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<2, 0, 3,   5,   ext>::value), 0);

    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, 3,   ext, ext>::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<1, 0, 3,   ext, ext>::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<2, 0, 3,   ext, ext>::value), 1);

    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, ext, 5,   ext>::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<1, 0, ext, 5,   ext>::value), 1);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<2, 0, ext, 5,   ext>::value), 1);

    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, ext, ext, 9  >::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<1, 0, ext, ext, 9  >::value), 1);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<2, 0, ext, ext, 9  >::value), 2);

    BOOST_TEST_EQ((dynamic_extent_tuple_index<0, 0, ext, ext, ext>::value), 0);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<1, 0, ext, ext, ext>::value), 1);
    BOOST_TEST_EQ((dynamic_extent_tuple_index<2, 0, ext, ext, ext>::value), 2);

    return boost::report_errors();
}

