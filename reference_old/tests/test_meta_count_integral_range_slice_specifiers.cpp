////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::experimental::detail::all_tag;
using std::experimental::detail::count_integral_range_slice_specifiers;

int main()
{
    ///////////////////////////////////////////////////////////////////////////

    BOOST_TEST_EQ((count_integral_range_slice_specifiers<>::value), 0);

    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        int
    >::value), 0);
    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        int, int
    >::value), 0);
    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        int, int, int
    >::value), 0);

    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        all_tag
    >::value), 1);
    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        all_tag, all_tag 
    >::value), 2);
    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        all_tag, all_tag, all_tag
    >::value), 3);

    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        all_tag, int,     int
    >::value), 1);
    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        int,     all_tag, int
    >::value), 1);
    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        int,     int,     all_tag
    >::value), 1);

    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        int,     all_tag, all_tag
    >::value), 2);
    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        all_tag, int,     all_tag 
    >::value), 2);
    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        all_tag, all_tag, int
    >::value), 2);

    BOOST_TEST_EQ((count_integral_range_slice_specifiers<
        all_tag, all_tag, all_tag
    >::value), 3);

    return boost::report_errors();
}
