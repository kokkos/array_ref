////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <vector>

#include <multi_array_ref>

using std::extent;
using std::experimental::dims;
using std::experimental::get_value;
using std::experimental::column_major_layout;
constexpr auto dyn = std::experimental::dynamic_dim;

template <std::size_t N, std::size_t X>
void test_1d_static()
{ // {{{
    static_assert(0 == (X % N), "X must be divisable by N");

    dims<X> d;
    dims<1>  s;

    dims<X/N> sub_d; 
    dims<N> sub_s;

    column_major_layout<int, decltype(s), dims<0> > l;

    column_major_layout<int, decltype(sub_s), dims<0> > sub_l;

    int dptr[X];

    // Set all elements to 42.
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        BOOST_TEST_EQ((l.index(d, i)), i);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i))), &(dptr[i])); 

        l.access(dptr, l.index(d, i)) = 42;

        BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 42); 
    }

    // Set every Nth element to 17. 
    for (auto i = 0; i < get_value<0>(sub_d); ++i)
    {
        BOOST_TEST_EQ((sub_l.index(sub_d, i)), get_value<0>(sub_s)*i);

        BOOST_TEST_EQ(&(sub_l.access(dptr, sub_l.index(sub_d, i)))
                    , &(dptr[get_value<0>(sub_s)*i])); 

        sub_l.access(dptr, sub_l.index(sub_d, i)) = 17;

        BOOST_TEST_EQ((sub_l.access(dptr, sub_l.index(sub_d, i))), 17);
    }

    // Check final structure. 
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        if (0 == (i % get_value<0>(sub_s)))
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 17);
        else
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 42);
    }
} // }}}

template <std::size_t N, std::size_t X>
void test_1d_dynamic()
{ // {{{
    static_assert(0 == (X % N), "X must be divisable by N");

    dims<dyn> d(X);
    dims<dyn> s(1);

    dims<dyn> sub_d(X/N); 
    dims<dyn> sub_s(N);

    column_major_layout<int, decltype(s), dims<0> >
        l(s, dims<0>());

    column_major_layout<int, decltype(sub_s), dims<0> >
        sub_l(sub_s, dims<0>());

    // Initialize all elements as 42.
    std::vector<int> data(get_value<0>(d), 42);
    int* dptr = data.data();

    // Set every Nth element to 17. 
    for (auto i = 0; i < get_value<0>(sub_d); ++i)
    {
        BOOST_TEST_EQ((sub_l.index(sub_d, i)), get_value<0>(sub_s)*i);

        BOOST_TEST_EQ(&(sub_l.access(dptr, sub_l.index(sub_d, i)))
                    , &(dptr[get_value<0>(sub_s)*i])); 

        sub_l.access(dptr, sub_l.index(sub_d, i)) = 17;

        BOOST_TEST_EQ((sub_l.access(dptr, sub_l.index(sub_d, i))), 17);
    }

    // Check final structure. 
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        if (0 == (i % get_value<0>(sub_s)))
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 17);
        else
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 42);
    }
} // }}}

int main()
{
    test_1d_static<1, 1>();
    test_1d_static<1, 30>();

    test_1d_static<2,  30>();
    test_1d_static<5,  30>();
    test_1d_static<10, 30>();
    test_1d_static<30, 30>();

    test_1d_dynamic<1, 1>();
    test_1d_dynamic<1, 30>();

    test_1d_dynamic<2,  30>();
    test_1d_dynamic<5,  30>();
    test_1d_dynamic<10, 30>();
    test_1d_dynamic<30, 30>();

    return boost::report_errors();
}

