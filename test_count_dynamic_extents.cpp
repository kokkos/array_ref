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
    using boost::detail::count_dynamic_extents;
    constexpr auto ext = boost::dynamic_extent;

    BOOST_TEST_EQ((count_dynamic_extents<             >::value), 0);
    BOOST_TEST_EQ((count_dynamic_extents<3            >::value), 0);
    BOOST_TEST_EQ((count_dynamic_extents<3,   5       >::value), 0);
    BOOST_TEST_EQ((count_dynamic_extents<3,   5,   9  >::value), 0);

    BOOST_TEST_EQ((count_dynamic_extents<ext, 5,   9  >::value), 1);
    BOOST_TEST_EQ((count_dynamic_extents<3,   ext, 9  >::value), 1);
    BOOST_TEST_EQ((count_dynamic_extents<3,   5,   ext>::value), 1);

    BOOST_TEST_EQ((count_dynamic_extents<3,   ext, ext>::value), 2);
    BOOST_TEST_EQ((count_dynamic_extents<ext, 5,   ext>::value), 2);
    BOOST_TEST_EQ((count_dynamic_extents<ext, ext, 9  >::value), 2);

    BOOST_TEST_EQ((count_dynamic_extents<ext, ext, ext>::value), 3);
    BOOST_TEST_EQ((count_dynamic_extents<ext, ext, ext>::value), 3);
    BOOST_TEST_EQ((count_dynamic_extents<ext, ext, ext>::value), 3);

    return boost::report_errors();
}

