////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::is_same;

using std::integral_constant;
using std::experimental::detail::integer_sequence;
using std::experimental::detail::integral_pair;

using std::experimental::detail::type_list;
using std::experimental::detail::type_list_sort;

using std::experimental::detail::make_key_value_type_list_from_integer_sequence;
using std::experimental::detail::make_integer_sequence_inverse_mapping;

int main()
{
    // type_list_sort {{{
    
    { // 01 {{{ 

        using A = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 10 {{{

        using A = type_list<
            integral_constant<int, 1>
          , integral_constant<int, 0>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((!is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 012 {{{

        using A = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 021 {{{

        using A = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 2>
          , integral_constant<int, 1>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((!is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 102 {{{

        using A = type_list<
            integral_constant<int, 1>
          , integral_constant<int, 0>
          , integral_constant<int, 2>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((!is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 120 {{{

        using A = type_list<
            integral_constant<int, 1>
          , integral_constant<int, 2>
          , integral_constant<int, 0>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((!is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 201 {{{

        using A = type_list<
            integral_constant<int, 2>
          , integral_constant<int, 0>
          , integral_constant<int, 1>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((!is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 210 {{{

        using A = type_list<
            integral_constant<int, 2>
          , integral_constant<int, 1>
          , integral_constant<int, 0>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((!is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 139 {{{

        using A = type_list<
            integral_constant<int, 1>
          , integral_constant<int, 3>
          , integral_constant<int, 9>
        >;

        using B = type_list<
            integral_constant<int, 1>
          , integral_constant<int, 3>
          , integral_constant<int, 9>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 391 {{{

        using A = type_list<
            integral_constant<int, 3>
          , integral_constant<int, 9>
          , integral_constant<int, 1>
        >;

        using B = type_list<
            integral_constant<int, 1>
          , integral_constant<int, 3>
          , integral_constant<int, 9>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((!is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);

    } // }}}

    { // 9123547806 {{{
        using A = type_list<
            integral_constant<int, 9>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
          , integral_constant<int, 3>
          , integral_constant<int, 5>
          , integral_constant<int, 4>
          , integral_constant<int, 7>
          , integral_constant<int, 8>
          , integral_constant<int, 0>
          , integral_constant<int, 6>
        >;

        using B = type_list<
            integral_constant<int, 0>
          , integral_constant<int, 1>
          , integral_constant<int, 2>
          , integral_constant<int, 3>
          , integral_constant<int, 4>
          , integral_constant<int, 5>
          , integral_constant<int, 6>
          , integral_constant<int, 7>
          , integral_constant<int, 8>
          , integral_constant<int, 9>
        >;

        using A_sorted = type_list_sort<A>::type;

        BOOST_TEST_EQ((!is_same<A, B>::value), true);
        BOOST_TEST_EQ((is_same<A_sorted, B>::value), true);
    } // }}}

    // }}}

    // make_key_value_type_list_from_integer_sequence {{{

    { // 01 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 0, 1>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 0>
          , integral_pair<std::size_t, int, 1, 1>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 10 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 1, 0>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 1>
          , integral_pair<std::size_t, int, 1, 0>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 012 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 0, 1, 2>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 0>
          , integral_pair<std::size_t, int, 1, 1>
          , integral_pair<std::size_t, int, 2, 2>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 021 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 0, 2, 1>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 0>
          , integral_pair<std::size_t, int, 1, 2>
          , integral_pair<std::size_t, int, 2, 1>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 102 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 1, 0, 2>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 1>
          , integral_pair<std::size_t, int, 1, 0>
          , integral_pair<std::size_t, int, 2, 2>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 120 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 1, 2, 0>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 1>
          , integral_pair<std::size_t, int, 1, 2>
          , integral_pair<std::size_t, int, 2, 0>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 201 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 2, 0, 1>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 2>
          , integral_pair<std::size_t, int, 1, 0>
          , integral_pair<std::size_t, int, 2, 1>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 210 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 2, 1, 0>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 2>
          , integral_pair<std::size_t, int, 1, 1>
          , integral_pair<std::size_t, int, 2, 0>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 0123 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 0, 1, 2, 3>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 0>
          , integral_pair<std::size_t, int, 1, 1>
          , integral_pair<std::size_t, int, 2, 2>
          , integral_pair<std::size_t, int, 3, 3>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 3210 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 3, 2, 1, 0>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 3>
          , integral_pair<std::size_t, int, 1, 2>
          , integral_pair<std::size_t, int, 2, 1>
          , integral_pair<std::size_t, int, 3, 0>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 1320 {{{

        using A = make_key_value_type_list_from_integer_sequence<
            integer_sequence<int, 1, 3, 2, 0>
        >::type;

        using B = type_list<
            integral_pair<std::size_t, int, 0, 1>
          , integral_pair<std::size_t, int, 1, 3>
          , integral_pair<std::size_t, int, 2, 2>
          , integral_pair<std::size_t, int, 3, 0>
        >;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    // }}} 

    // make_integer_sequence_inverse_mapping {{{

    { // 01 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 0, 1>
        >::type;

        using B = integer_sequence<std::size_t, 0, 1>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 10 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 1, 0>
        >::type;

        using B = integer_sequence<std::size_t, 1, 0>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 012 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 0, 1, 2>
        >::type;

        using B = integer_sequence<std::size_t, 0, 1, 2>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 021 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 0, 2, 1>
        >::type;

        using B = integer_sequence<std::size_t, 0, 2, 1>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 102 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 1, 0, 2>
        >::type;

        using B = integer_sequence<std::size_t, 1, 0, 2>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 120 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 1, 2, 0>
        >::type;

        using B = integer_sequence<std::size_t, 2, 0, 1>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 201 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 2, 0, 1>
        >::type;

        using B = integer_sequence<std::size_t, 1, 2, 0>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 210 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 2, 1, 0>
        >::type;

        using B = integer_sequence<std::size_t, 2, 1, 0>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 0123 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 0, 1, 2, 3>
        >::type;

        using B = integer_sequence<std::size_t, 0, 1, 2, 3>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 3210 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 3, 2, 1, 0>
        >::type;

        using B = integer_sequence<std::size_t, 3, 2, 1, 0>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    { // 1320 {{{

        using A = make_integer_sequence_inverse_mapping<
            integer_sequence<int, 1, 3, 2, 0>
        >::type;

        using B = integer_sequence<std::size_t, 3, 0, 2, 1>;

        BOOST_TEST_EQ((is_same<A, B>::value), true);

    } // }}}

    // }}}

    return boost::report_errors();
}

