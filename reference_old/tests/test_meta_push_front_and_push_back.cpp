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
using std::experimental::detail::integer_sequence;
using std::experimental::detail::type_list;

using std::experimental::detail::dimensions_push_front;
using std::experimental::detail::integer_sequence_push_front;
using std::experimental::detail::type_list_push_front;

using std::experimental::detail::dimensions_push_back;
using std::experimental::detail::integer_sequence_push_back;
using std::experimental::detail::type_list_push_back;

int main()
{
    { // dimensions_push_front {{{

        BOOST_TEST_EQ((is_same<
            dimensions_push_front<dyn, dimensions<> >
          , dimensions<dyn>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<dyn, dimensions<dyn> >
          , dimensions<dyn, dyn>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<dyn, dimensions<dyn, dyn> >
          , dimensions<dyn, dyn, dyn>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            dimensions_push_front<3, dimensions<> >
          , dimensions<3>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<3, dimensions<5> >
          , dimensions<3, 5>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<3, dimensions<5, 9> >
          , dimensions<3, 5, 9>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            dimensions_push_front<dyn, dimensions<5> >
          , dimensions<dyn, 5>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<5, dimensions<dyn> >
          , dimensions<5, dyn>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            dimensions_push_front<dyn, dimensions<5, 9> >
          , dimensions<dyn, 5, 9>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<3, dimensions<dyn, 9> >
          , dimensions<3, dyn, 9>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<3, dimensions<5, dyn> >
          , dimensions<3, 5, dyn>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            dimensions_push_front<3, dimensions<dyn, dyn> >
          , dimensions<3, dyn, dyn>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<dyn, dimensions<5, dyn> >
          , dimensions<dyn, 5, dyn>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_front<dyn, dimensions<dyn, 9> >
          , dimensions<dyn, dyn, 9>
        >::value), true);

    } // }}}

    { // dimensions_push_back {{{

        BOOST_TEST_EQ((is_same<
            dimensions_push_back<dyn, dimensions<> >
          , dimensions<dyn>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<dyn, dimensions<dyn> >
          , dimensions<dyn, dyn>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<dyn, dimensions<dyn, dyn> >
          , dimensions<dyn, dyn, dyn>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            dimensions_push_back<3, dimensions<> >
          , dimensions<3>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<5, dimensions<3> >
          , dimensions<3, 5>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<9, dimensions<3, 5> >
          , dimensions<3, 5, 9>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            dimensions_push_back<5, dimensions<dyn> >
          , dimensions<dyn, 5>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<dyn, dimensions<5> >
          , dimensions<5, dyn>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            dimensions_push_back<9, dimensions<dyn, 5> >
          , dimensions<dyn, 5, 9>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<9, dimensions<3, dyn> >
          , dimensions<3, dyn, 9>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<dyn, dimensions<3, 5> >
          , dimensions<3, 5, dyn>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            dimensions_push_back<dyn, dimensions<3, dyn> >
          , dimensions<3, dyn, dyn>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<dyn, dimensions<dyn, 5> >
          , dimensions<dyn, 5, dyn>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            dimensions_push_back<9, dimensions<dyn, dyn> >
          , dimensions<dyn, dyn, 9>
        >::value), true);

    } // }}}

    { // integer_sequence_push_front {{{

        BOOST_TEST_EQ((is_same<
            integer_sequence_push_front<
                std::size_t, 0, integer_sequence<std::size_t>
            >::type
          , integer_sequence<std::size_t, 0>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            integer_sequence_push_front<
                std::size_t, 0, integer_sequence<std::size_t, 0>
            >::type
          , integer_sequence<std::size_t, 0, 0>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            integer_sequence_push_front<
                std::size_t, 0, integer_sequence<std::size_t, 0, 0>
            >::type
          , integer_sequence<std::size_t, 0, 0, 0>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            integer_sequence_push_front<
                std::size_t, 0, integer_sequence<std::size_t, 1>
            >::type
          , integer_sequence<std::size_t, 0, 1>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            integer_sequence_push_front<
                std::size_t, 0, integer_sequence<std::size_t, 1, 2>
            >::type
          , integer_sequence<std::size_t, 0, 1, 2>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            integer_sequence_push_front<
                int, 3, integer_sequence<int, 5>
            >::type
          , integer_sequence<int, 3, 5>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            integer_sequence_push_front<
                int, 3, integer_sequence<int, 5, 9>
            >::type
          , integer_sequence<int, 3, 5, 9>
        >::value), true);

    } // }}}

    { // integer_sequence_push_back {{{

        BOOST_TEST_EQ((is_same<
            integer_sequence_push_back<
                std::size_t, 0, integer_sequence<std::size_t>
            >::type
          , integer_sequence<std::size_t, 0>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            integer_sequence_push_back<
                std::size_t, 0, integer_sequence<std::size_t, 0>
            >::type
          , integer_sequence<std::size_t, 0, 0>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            integer_sequence_push_back<
                std::size_t, 0, integer_sequence<std::size_t, 0, 0>
            >::type
          , integer_sequence<std::size_t, 0, 0, 0>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            integer_sequence_push_back<
                std::size_t, 1, integer_sequence<std::size_t, 0>
            >::type
          , integer_sequence<std::size_t, 0, 1>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            integer_sequence_push_back<
                std::size_t, 2, integer_sequence<std::size_t, 0, 1>
            >::type
          , integer_sequence<std::size_t, 0, 1, 2>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            integer_sequence_push_back<
                int, 5, integer_sequence<int, 3>
            >::type
          , integer_sequence<int, 3, 5>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            integer_sequence_push_back<
                int, 9, integer_sequence<int, 3, 5>
            >::type
          , integer_sequence<int, 3, 5, 9>
        >::value), true);

    } // }}}

    { // type_list_push_front {{{

        BOOST_TEST_EQ((is_same<
            type_list_push_front<int, type_list<> >::type
          , type_list<int>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            type_list_push_front<int, type_list<int> >::type
          , type_list<int, int>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            type_list_push_front<int, type_list<int, int> >::type
          , type_list<int, int, int>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            type_list_push_front<int, type_list<char> >::type
          , type_list<int, char>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            type_list_push_front<int, type_list<char, bool> >::type
          , type_list<int, char, bool>
        >::value), true);

    } // }}}

    { // type_list_push_back {{{

        BOOST_TEST_EQ((is_same<
            type_list_push_back<int, type_list<> >::type
          , type_list<int>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            type_list_push_back<int, type_list<int> >::type
          , type_list<int, int>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            type_list_push_back<int, type_list<int, int> >::type
          , type_list<int, int, int>
        >::value), true);

        BOOST_TEST_EQ((is_same<
            type_list_push_back<char, type_list<int> >::type
          , type_list<int, char>
        >::value), true);
        BOOST_TEST_EQ((is_same<
            type_list_push_back<bool, type_list<int, char> >::type
          , type_list<int, char, bool>
        >::value), true);

    } // }}}

    return boost::report_errors();
}

