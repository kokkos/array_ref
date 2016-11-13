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
using std::experimental::detail::make_filled_dims_t;

int main()
{
    BOOST_TEST((is_same<
        make_filled_dims_t<0, 1>, dimensions<>
    >::value));

    BOOST_TEST((is_same<
        make_filled_dims_t<1, 1  >, dimensions<1>
    >::value));
    BOOST_TEST((is_same<
        make_filled_dims_t<1, 0  >, dimensions<0>
    >::value));
    BOOST_TEST((is_same<
        make_filled_dims_t<1, dyn>, dimensions<dyn>
    >::value));

    BOOST_TEST((is_same<
        make_filled_dims_t<2, 1  >, dimensions<1,   1  >
    >::value));
    BOOST_TEST((is_same<
        make_filled_dims_t<2, 0  >, dimensions<0,   0  >
    >::value));
    BOOST_TEST((is_same<
        make_filled_dims_t<2, dyn>, dimensions<dyn, dyn>
    >::value));

    BOOST_TEST((is_same<
        make_filled_dims_t<3, 1  >, dimensions<1,   1,   1  >
    >::value));
    BOOST_TEST((is_same<
        make_filled_dims_t<3, 0  >, dimensions<0,   0,   0  >
    >::value));
    BOOST_TEST((is_same<
        make_filled_dims_t<3, dyn>, dimensions<dyn, dyn, dyn>
    >::value));

    return boost::report_errors();
}

