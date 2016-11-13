////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::experimental::dyn;
using std::experimental::detail::index_into_dynamic_dims;

int main()
{
    BOOST_TEST_EQ((index_into_dynamic_dims(0               )), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(0, 3            )), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(0, 3,   5       )), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(0, 3,   5,   9  )), 0);

    BOOST_TEST_EQ((index_into_dynamic_dims(0, dyn, 5,   9  )), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(1, dyn, 5,   9  )), 1);
    BOOST_TEST_EQ((index_into_dynamic_dims(2, dyn, 5,   9  )), 1);

    BOOST_TEST_EQ((index_into_dynamic_dims(0, 3,   dyn, 9  )), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(1, 3,   dyn, 9  )), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(2, 3,   dyn, 9  )), 1);

    BOOST_TEST_EQ((index_into_dynamic_dims(0, 3,   5,   dyn)), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(1, 3,   5,   dyn)), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(2, 3,   5,   dyn)), 0);

    BOOST_TEST_EQ((index_into_dynamic_dims(0, 3,   dyn, dyn)), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(1, 3,   dyn, dyn)), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(2, 3,   dyn, dyn)), 1);

    BOOST_TEST_EQ((index_into_dynamic_dims(0, dyn, 5,   dyn)), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(1, dyn, 5,   dyn)), 1);
    BOOST_TEST_EQ((index_into_dynamic_dims(2, dyn, 5,   dyn)), 1);

    BOOST_TEST_EQ((index_into_dynamic_dims(0, dyn, dyn, 9  )), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(1, dyn, dyn, 9  )), 1);
    BOOST_TEST_EQ((index_into_dynamic_dims(2, dyn, dyn, 9  )), 2);

    BOOST_TEST_EQ((index_into_dynamic_dims(0, dyn, dyn, dyn)), 0);
    BOOST_TEST_EQ((index_into_dynamic_dims(1, dyn, dyn, dyn)), 1);
    BOOST_TEST_EQ((index_into_dynamic_dims(2, dyn, dyn, dyn)), 2);

    return boost::report_errors();
}

