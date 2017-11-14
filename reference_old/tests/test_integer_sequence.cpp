////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include "detail/integer_sequence.hpp"

using std::true_type;
using std::false_type;
using std::is_same;

using std::experimental::detail::integer_sequence;

using std::experimental::detail::make_integer_sequence;
using std::experimental::detail::make_reversed_integer_sequence;

int main()
{
    ///////////////////////////////////////////////////////////////////////////
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 0>
      , integer_sequence<int>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 1>
      , integer_sequence<int, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 2>
      , integer_sequence<int, 0, 1>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 3>
      , integer_sequence<int, 0, 1, 2>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 4>
      , integer_sequence<int, 0, 1, 2, 3>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 5>
      , integer_sequence<int, 0, 1, 2, 3, 4>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 6>
      , integer_sequence<int, 0, 1, 2, 3, 4, 5>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 7>
      , integer_sequence<int, 0, 1, 2, 3, 4, 5, 6>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 8>
      , integer_sequence<int, 0, 1, 2, 3, 4, 5, 6, 7>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 9>
      , integer_sequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_integer_sequence<int, 10>
      , integer_sequence<int, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9>
    >::value), true);

    ///////////////////////////////////////////////////////////////////////////
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 0>
      , integer_sequence<int>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 1>
      , integer_sequence<int, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 2>
      , integer_sequence<int, 1, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 3>
      , integer_sequence<int, 2, 1, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 4>
      , integer_sequence<int, 3, 2, 1, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 5>
      , integer_sequence<int, 4, 3, 2, 1, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 6>
      , integer_sequence<int, 5, 4, 3, 2, 1, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 7>
      , integer_sequence<int, 6, 5, 4, 3, 2, 1, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 8>
      , integer_sequence<int, 7, 6, 5, 4, 3, 2, 1, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 9>
      , integer_sequence<int, 8, 7, 6, 5, 4, 3, 2, 1, 0>
    >::value), true);
    BOOST_TEST_EQ((is_same<
        make_reversed_integer_sequence<int, 10>
      , integer_sequence<int, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0>
    >::value), true);

    return boost::report_errors();
}
