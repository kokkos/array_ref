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
    using boost::extents;
    using boost::get_value;
    constexpr auto ext = boost::dynamic_extent;

    {
        extents<3            > e0;
        extents<3,   5       > e1;
        extents<3,   5,   9  > e2;

        BOOST_TEST_EQ((get_value<0>(e0)), 3);

        BOOST_TEST_EQ((get_value<0>(e1)), 3);
        BOOST_TEST_EQ((get_value<1>(e1)), 5);

        BOOST_TEST_EQ((get_value<0>(e2)), 3);
        BOOST_TEST_EQ((get_value<1>(e2)), 5);
        BOOST_TEST_EQ((get_value<2>(e2)), 9);
    }

    {
        extents<ext, 5,   9  > e0(3);
        extents<3,   ext, 9  > e1(5);
        extents<3,   5,   ext> e2(9);

        BOOST_TEST_EQ((get_value<0>(e0)), 3);
        BOOST_TEST_EQ((get_value<1>(e0)), 5);
        BOOST_TEST_EQ((get_value<2>(e0)), 9);

        BOOST_TEST_EQ((get_value<0>(e1)), 3);
        BOOST_TEST_EQ((get_value<1>(e1)), 5);
        BOOST_TEST_EQ((get_value<2>(e1)), 9);

        BOOST_TEST_EQ((get_value<0>(e2)), 3);
        BOOST_TEST_EQ((get_value<1>(e2)), 5);
        BOOST_TEST_EQ((get_value<2>(e2)), 9);
    }

    {
        extents<3,   ext, ext> e0(5, 9);
        extents<ext, 5,   ext> e1(3, 9);
        extents<ext, ext, 9  > e2(3, 5);

        BOOST_TEST_EQ((get_value<0>(e0)), 3);
        BOOST_TEST_EQ((get_value<1>(e0)), 5);
        BOOST_TEST_EQ((get_value<2>(e0)), 9);

        BOOST_TEST_EQ((get_value<0>(e1)), 3);
        BOOST_TEST_EQ((get_value<1>(e1)), 5);
        BOOST_TEST_EQ((get_value<2>(e1)), 9);

        BOOST_TEST_EQ((get_value<0>(e2)), 3);
        BOOST_TEST_EQ((get_value<1>(e2)), 5);
        BOOST_TEST_EQ((get_value<2>(e2)), 9);
    }

    {
        extents<ext>           e0(3);
        extents<ext, ext>      e1(3, 5);
        extents<ext, ext, ext> e2(3, 5, 9);

        BOOST_TEST_EQ((get_value<0>(e0)), 3);

        BOOST_TEST_EQ((get_value<0>(e1)), 3);
        BOOST_TEST_EQ((get_value<1>(e1)), 5);

        BOOST_TEST_EQ((get_value<0>(e2)), 3);
        BOOST_TEST_EQ((get_value<1>(e2)), 5);
        BOOST_TEST_EQ((get_value<2>(e2)), 9);
    }

    return boost::report_errors();
}

