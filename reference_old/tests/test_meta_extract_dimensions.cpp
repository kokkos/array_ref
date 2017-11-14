////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::is_same;

using std::experimental::dyn;
using std::experimental::dimensions;
using std::experimental::extract_dimensions_t;

int main()
{
    BOOST_TEST((is_same<
        extract_dimensions_t<int>, dimensions<>
    >::value));

    BOOST_TEST((is_same<
        extract_dimensions_t<int[ ]>, dimensions<dyn>
    >::value));

    BOOST_TEST((is_same<
        extract_dimensions_t<int[3]>, dimensions<3  >
    >::value));

    BOOST_TEST((is_same<
        extract_dimensions_t<int[ ][5]>, dimensions<dyn, 5>
    >::value));
    BOOST_TEST((is_same<
        extract_dimensions_t<int[3][5]>, dimensions<3,   5>
    >::value));

    BOOST_TEST((is_same<
        extract_dimensions_t<int[ ][5][9]>, dimensions<dyn, 5, 9>
    >::value));
    BOOST_TEST((is_same<
        extract_dimensions_t<int[3][5][9]>, dimensions<3,   5, 9>
    >::value));

    return boost::report_errors();
}

