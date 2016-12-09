////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::is_same;

using std::experimental::index_sequence;

using std::experimental::dyn;
using std::experimental::detail::make_dynamic_dims_indices_t;

int main()
{
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<             >, index_sequence<>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<3            >, index_sequence<>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<3,   5       >, index_sequence<>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<3,   5,   9  >, index_sequence<>
    >::value));

    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<dyn          >, index_sequence<0>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<dyn, dyn     >, index_sequence<0, 1>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<dyn, dyn, dyn>, index_sequence<0, 1, 2>
    >::value));

    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<dyn, 5,   9  >, index_sequence<0>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<3,   dyn, 9  >, index_sequence<1>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<3,   5,   dyn>, index_sequence<2>
    >::value));

    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<3,   dyn, dyn>, index_sequence<1, 2>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<dyn, 5,   dyn>, index_sequence<0, 2>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_indices_t<dyn, dyn, 9  >, index_sequence<0, 1>
    >::value));

    return boost::report_errors();
}

