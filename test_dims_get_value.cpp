////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <multi_array_ref>

int main()
{
    using std::experimental::dims;
    using std::experimental::get_value;
    constexpr auto dyn = std::experimental::dynamic_dim;

    {
        dims<3            > e0;
        dims<3,   5       > e1;
        dims<3,   5,   9  > e2;

        BOOST_TEST_EQ((get_value<0>(e0)), 3);

        BOOST_TEST_EQ((get_value<0>(e1)), 3);
        BOOST_TEST_EQ((get_value<1>(e1)), 5);

        BOOST_TEST_EQ((get_value<0>(e2)), 3);
        BOOST_TEST_EQ((get_value<1>(e2)), 5);
        BOOST_TEST_EQ((get_value<2>(e2)), 9);
    }

    {
        dims<dyn, 5,   9  > e0(3);
        dims<3,   dyn, 9  > e1(5);
        dims<3,   5,   dyn> e2(9);

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
        dims<3,   dyn, dyn> e0(5, 9);
        dims<dyn, 5,   dyn> e1(3, 9);
        dims<dyn, dyn, 9  > e2(3, 5);

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
        dims<dyn>           e0(3);
        dims<dyn, dyn>      e1(3, 5);
        dims<dyn, dyn, dyn> e2(3, 5, 9);

        BOOST_TEST_EQ((get_value<0>(e0)), 3);

        BOOST_TEST_EQ((get_value<0>(e1)), 3);
        BOOST_TEST_EQ((get_value<1>(e1)), 5);

        BOOST_TEST_EQ((get_value<0>(e2)), 3);
        BOOST_TEST_EQ((get_value<1>(e2)), 5);
        BOOST_TEST_EQ((get_value<2>(e2)), 9);
    }

    return boost::report_errors();
}

