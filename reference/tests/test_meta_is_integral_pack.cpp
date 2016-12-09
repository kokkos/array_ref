////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::experimental::detail::is_integral_pack;

int main()
{
    BOOST_TEST_EQ((is_integral_pack<>::value), true);

    BOOST_TEST_EQ((is_integral_pack<int  >::value), true);
    BOOST_TEST_EQ((is_integral_pack<float>::value), false);

    BOOST_TEST_EQ((is_integral_pack<int  , int  , int  >::value), true);
    BOOST_TEST_EQ((is_integral_pack<float, int  , int  >::value), false);
    BOOST_TEST_EQ((is_integral_pack<int  , float, int  >::value), false);
    BOOST_TEST_EQ((is_integral_pack<int  , int  , float>::value), false);
    BOOST_TEST_EQ((is_integral_pack<float, float, float>::value), false);

    return boost::report_errors();
}
