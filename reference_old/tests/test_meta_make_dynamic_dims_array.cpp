////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::is_same;
using std::array;

using std::experimental::dyn;
using std::experimental::detail::make_dynamic_dims_array_t;

int main()
{
    // See notes in the definition of make_dynamic_dims_array, when there are no
    // dynamic extents we change the element type to unsigned char to ensure
    // that the size of the array is 1.
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<             >, array<unsigned char, 0>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<3            >, array<unsigned char, 0>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<3,   5       >, array<unsigned char, 0>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<3,   5,   9  >, array<unsigned char, 0>
    >::value));

    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<dyn          >, array<std::size_t, 1>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<dyn, dyn     >, array<std::size_t, 2>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<dyn, dyn, dyn>, array<std::size_t, 3>
    >::value));

    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<dyn, 5,   9  >, array<std::size_t, 1>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<3,   dyn, 9  >, array<std::size_t, 1>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<3,   5,   dyn>, array<std::size_t, 1>
    >::value));

    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<3,   dyn, dyn>, array<std::size_t, 2>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<dyn, 5,   dyn>, array<std::size_t, 2>
    >::value));
    BOOST_TEST((is_same<
        make_dynamic_dims_array_t<dyn, dyn, 9  >, array<std::size_t, 2>
    >::value));

    return boost::report_errors();
}

