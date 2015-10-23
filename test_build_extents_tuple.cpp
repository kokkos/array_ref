////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include "view.hpp"

int main()
{
    using std::rank;
    using std::extent;

    using boost::extents;
    constexpr auto ext = boost::dynamic_extent;

    BOOST_TEST((std::is_same<
        extents<       >::dynamic_extents_type
      , std::tuple<>
    >::value));
    BOOST_TEST((std::is_same<
        extents<3      >::dynamic_extents_type
      , std::tuple<>
    >::value));
    BOOST_TEST((std::is_same<
        extents<3, 5   >::dynamic_extents_type
      , std::tuple<>
    >::value));
    BOOST_TEST((std::is_same<
        extents<3, 5, 9>::dynamic_extents_type
      , std::tuple<>
    >::value));

    BOOST_TEST((std::is_same<
        extents<ext          >::dynamic_extents_type
      , std::tuple<std::size_t>
    >::value));
    BOOST_TEST((std::is_same<
        extents<ext, ext     >::dynamic_extents_type
      , std::tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((std::is_same<
        extents<ext, ext, ext>::dynamic_extents_type
      , std::tuple<std::size_t, std::size_t, std::size_t>
    >::value));

    BOOST_TEST((std::is_same<
        extents<ext, 5,   9  >::dynamic_extents_type
      , std::tuple<std::size_t>
    >::value));
    BOOST_TEST((std::is_same<
        extents<3,   ext, 9  >::dynamic_extents_type
      , std::tuple<std::size_t>
    >::value));
    BOOST_TEST((std::is_same<
        extents<3,   5,   ext>::dynamic_extents_type
      , std::tuple<std::size_t>
    >::value));

    BOOST_TEST((std::is_same<
        extents<3,   ext, ext>::dynamic_extents_type
      , std::tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((std::is_same<
        extents<ext, 5,   ext>::dynamic_extents_type
      , std::tuple<std::size_t, std::size_t>
    >::value));
    BOOST_TEST((std::is_same<
        extents<ext, ext, 9  >::dynamic_extents_type
      , std::tuple<std::size_t, std::size_t>
    >::value));

    return boost::report_errors();
}

