////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::rank;
using std::extent;

using std::experimental::dyn;
using std::experimental::detail::dynamic_extent;

int main()
{
    BOOST_TEST_EQ((dynamic_extent(0)), 0);
    BOOST_TEST_EQ((dynamic_extent(1)), 0);
    BOOST_TEST_EQ((dynamic_extent(2)), 0);
    
    BOOST_TEST_EQ((dynamic_extent(0, 3)), 3);
    BOOST_TEST_EQ((dynamic_extent(1, 3)), 0);

    BOOST_TEST_EQ((dynamic_extent(0, 0)), 0);
    BOOST_TEST_NE((dynamic_extent(0, 0)), dyn);
    BOOST_TEST_EQ((dynamic_extent(1, 0)), 0);
    BOOST_TEST_NE((dynamic_extent(1, 0)), dyn);
    
    BOOST_TEST_EQ((dynamic_extent(0, 3, 5)), 3);
    BOOST_TEST_EQ((dynamic_extent(1, 3, 5)), 5);
    BOOST_TEST_EQ((dynamic_extent(2, 3, 5)), 0);

    BOOST_TEST_EQ((dynamic_extent(0, dyn, 5)), dyn);
    BOOST_TEST_NE((dynamic_extent(0, dyn, 5)), 0);
    BOOST_TEST_EQ((dynamic_extent(1, dyn, 5)), 5);
    BOOST_TEST_EQ((dynamic_extent(2, dyn, 5)), 0);

    return boost::report_errors();
}

