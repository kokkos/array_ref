////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <array_ref>

int main()
{
    using std::is_same;
    using std::tuple;

    using std::experimental::dimensions;
    constexpr auto dyn = std::experimental::dynamic_dimension;

    BOOST_TEST((is_same<
        dimensions<       >::dynamic_dims_type
      , tuple<>
    >::value));
    BOOST_TEST((is_same<
        dimensions<3      >::dynamic_dims_type
      , tuple<>
    >::value));
    BOOST_TEST((is_same<
        dimensions<3, 5   >::dynamic_dims_type
      , tuple<>
    >::value));
    BOOST_TEST((is_same<
        dimensions<3, 5, 9>::dynamic_dims_type
      , tuple<>
    >::value));

    BOOST_TEST((is_same<
        dimensions<dyn          >::dynamic_dims_type
      , tuple<std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<dyn, dyn     >::dynamic_dims_type
      , tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<dyn, dyn, dyn>::dynamic_dims_type
      , tuple<std::size_t, std::size_t, std::size_t>
    >::value));

    BOOST_TEST((is_same<
        dimensions<0            >::dynamic_dims_type
      , tuple<std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<0,   0       >::dynamic_dims_type
      , tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<0,   0,   0  >::dynamic_dims_type
      , tuple<std::size_t, std::size_t, std::size_t>
    >::value));

    BOOST_TEST((is_same<
        dimensions<dyn, 5,   9  >::dynamic_dims_type
      , tuple<std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<3,   dyn, 9  >::dynamic_dims_type
      , tuple<std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<3,   5,   dyn>::dynamic_dims_type
      , tuple<std::size_t>
    >::value));

    BOOST_TEST((is_same<
        dimensions<0,   5,   9  >::dynamic_dims_type
      , tuple<std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<3,   0,   9  >::dynamic_dims_type
      , tuple<std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<3,   5,   0  >::dynamic_dims_type
      , tuple<std::size_t>
    >::value));

    BOOST_TEST((is_same<
        dimensions<3,   dyn, dyn>::dynamic_dims_type
      , tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<dyn, 5,   dyn>::dynamic_dims_type
      , tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<dyn, dyn, 9  >::dynamic_dims_type
      , tuple<std::size_t, std::size_t>
    >::value));

    BOOST_TEST((is_same<
        dimensions<3,   0,   0  >::dynamic_dims_type
      , tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<0,   5,   0  >::dynamic_dims_type
      , tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((is_same<
        dimensions<0,   0,   9  >::dynamic_dims_type
      , tuple<std::size_t, std::size_t>
    >::value));

    return boost::report_errors();
}

