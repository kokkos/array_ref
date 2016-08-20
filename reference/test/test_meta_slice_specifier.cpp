////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <array_ref>

using std::true_type;
using std::false_type;
using std::is_convertible;

using std::initializer_list;
using std::pair;
using std::tuple;
using std::array;

using std::experimental::detail::all_tag;
using std::experimental::detail::is_slice_specifier;
using std::experimental::detail::is_integral_range_slice_specifier;

int main()
{
    ///////////////////////////////////////////////////////////////////////////

    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<int>,    false_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<int[2]>, false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<initializer_list<int> >,   true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<initializer_list<int&> >,  true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<initializer_list<float> >, false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<pair<int, int> >,      true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<pair<int, unsigned> >, true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<pair<int&, int&> >,    true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<pair<int&, int> >,     true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<pair<float, float> >,  false_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<pair<int, float> >,    false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<tuple<int, int> >,      true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<tuple<int, unsigned> >, true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<tuple<int&, int&> >,    true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<tuple<int&, int> >,     true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<tuple<float, float> >,  false_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<tuple<int, float> >,    false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<array<int, 2> >,   true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<array<int&, 2> >,  true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<array<float, 2> >, false_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<array<int, 3> >,   false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_integral_range_slice_specifier<all_tag>, true_type
    >::value), true);

    ///////////////////////////////////////////////////////////////////////////

    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<int>,   true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<float>, false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<initializer_list<int> >,   true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<initializer_list<float> >, false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<pair<int, int> >,      true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<pair<float, float> >,  false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<tuple<int, int> >,      true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<tuple<float, float> >,  false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<array<int, 2> >,   true_type
    >::value), true);
    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<array<float, 2> >, false_type
    >::value), true);

    BOOST_TEST_EQ((is_convertible<
        is_slice_specifier<all_tag>, true_type
    >::value), true);

    return boost::report_errors();
}
