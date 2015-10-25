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
    using std::experimental::detail::pack_is_integral;

    BOOST_TEST_EQ((pack_is_integral<int  >::value), true);
    BOOST_TEST_EQ((pack_is_integral<float>::value), false);

    BOOST_TEST_EQ((pack_is_integral<int  , int  , int  >::value), true);
    BOOST_TEST_EQ((pack_is_integral<float, int  , int  >::value), false);
    BOOST_TEST_EQ((pack_is_integral<int  , float, int  >::value), false);
    BOOST_TEST_EQ((pack_is_integral<int  , int  , float>::value), false);
    BOOST_TEST_EQ((pack_is_integral<float, float, float>::value), false);

    return boost::report_errors();
}
