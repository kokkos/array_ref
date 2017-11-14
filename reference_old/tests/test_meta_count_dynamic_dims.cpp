////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::experimental::dyn;
using std::experimental::detail::count_dynamic_dims;

int main()
{
    BOOST_TEST_EQ((count_dynamic_dims<             >::value), 0);

    BOOST_TEST_EQ((count_dynamic_dims<3            >::value), 0);
    BOOST_TEST_EQ((count_dynamic_dims<3,   5       >::value), 0);
    BOOST_TEST_EQ((count_dynamic_dims<3,   5,   9  >::value), 0);

    BOOST_TEST_EQ((count_dynamic_dims<dyn          >::value), 1);
    BOOST_TEST_EQ((count_dynamic_dims<dyn, dyn     >::value), 2);
    BOOST_TEST_EQ((count_dynamic_dims<dyn, dyn, dyn>::value), 3);

    BOOST_TEST_EQ((count_dynamic_dims<dyn, 5,   9  >::value), 1);
    BOOST_TEST_EQ((count_dynamic_dims<3,   dyn, 9  >::value), 1);
    BOOST_TEST_EQ((count_dynamic_dims<3,   5,   dyn>::value), 1);

    BOOST_TEST_EQ((count_dynamic_dims<3,   dyn, dyn>::value), 2);
    BOOST_TEST_EQ((count_dynamic_dims<dyn, 5,   dyn>::value), 2);
    BOOST_TEST_EQ((count_dynamic_dims<dyn, dyn, 9  >::value), 2);

    return boost::report_errors();
}

