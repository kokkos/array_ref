////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <vector>

#include <array_ref>

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
        auto const true_idx = i*get_value<0>(sub_s);

        BOOST_TEST_EQ((sub_l.index(sub_d, i)), true_idx);

        BOOST_TEST_EQ(&(sub_l.access(dptr, sub_l.index(sub_d, i)))
                    , &(dptr[true_idx])); 

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
        auto const true_idx = i*get_value<0>(sub_s);

        BOOST_TEST_EQ((sub_l.index(sub_d, i)), true_idx);

        BOOST_TEST_EQ(&(sub_l.access(dptr, sub_l.index(sub_d, i)))
                    , &(dptr[true_idx])); 

        sub_l.access(dptr, sub_l.index(sub_d, i)) = 17;

        BOOST_TEST_EQ((sub_l.access(dptr, sub_l.index(sub_d, i))), 17);

        // Bounds-checking.
        BOOST_TEST_EQ((data.at(sub_l.index(sub_d, i))), 17);
    }

    // Check final structure.
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        if (0 == (i % get_value<0>(sub_s)))
        {
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 17);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(d, i))), 17);
        }
        else
        {
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 42);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(d, i))), 42);
        }
    }
} // }}}

template <std::size_t N, std::size_t M, std::size_t X, std::size_t Y>
void test_2d_static()
{ // {{{
    static_assert(0 == (X % N), "X must be divisable by N");
    static_assert(0 == (Y % M), "Y must be divisable by M");

    dims<X, Y> d;
    dims<1, 1>  s;

    dims<X/N, Y/M> sub_d; 
    dims<N,   M  > sub_s;

    column_major_layout<int, decltype(s), dims<0, 0> > l;

    column_major_layout<int, decltype(sub_s), dims<0, 0> > sub_l;

    int dptr[X*Y];

    // Set all elements to 42.
    for (auto j = 0; j < get_value<1>(d); ++j)
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        auto const true_idx = i + j*get_value<0>(d);

        BOOST_TEST_EQ((l.index(d, i, j)), true_idx);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i, j))), &(dptr[true_idx]));

        l.access(dptr, l.index(d, i, j)) = 42;

        BOOST_TEST_EQ((l.access(dptr, l.index(d, i, j))), 42);
    }

    // Set every (Nth, Mth) element to 17.
    for (auto j = 0; j < get_value<1>(sub_d); ++j)
    for (auto i = 0; i < get_value<0>(sub_d); ++i)
    {
        auto const true_idx = (i + j*get_value<1>(sub_s)*get_value<0>(sub_d))
                            * get_value<0>(sub_s);

        BOOST_TEST_EQ((sub_l.index(sub_d, i, j)), true_idx);

        BOOST_TEST_EQ(&(sub_l.access(dptr, sub_l.index(sub_d, i, j)))
                    , &(dptr[true_idx])); 

        sub_l.access(dptr, sub_l.index(sub_d, i, j)) = 17;

        BOOST_TEST_EQ((sub_l.access(dptr, sub_l.index(sub_d, i, j))), 17);
    }

    // Check final structure. 
    for (auto j = 0; j < get_value<1>(d); ++j)
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        if (  (0 == (i % get_value<0>(sub_s)))
           && (0 == (j % get_value<1>(sub_s)))
           )
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i, j))), 17);
        else
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i, j))), 42);
    }
} // }}}

template <std::size_t N, std::size_t M, std::size_t X, std::size_t Y>
void test_2d_dynamic()
{ // {{{
    static_assert(0 == (X % N), "X must be divisable by N");
    static_assert(0 == (Y % M), "Y must be divisable by M");

    dims<dyn, dyn> d(X, Y);
    dims<dyn, dyn> s(1, 1);

    dims<dyn, dyn> sub_d(X/N, Y/M); 
    dims<dyn, dyn> sub_s(N,   M  );;

    column_major_layout<int, decltype(s), dims<0, 0> >
        l(s, dims<0, 0>());

    column_major_layout<int, decltype(sub_s), dims<0, 0> >
        sub_l(sub_s, dims<0, 0>());

    // Initialize all elements as 42.
    std::vector<int> data(get_value<0>(d)*get_value<1>(d), 42);
    int* dptr = data.data();

    // Set every (Nth, Mth) element to 17.
    for (auto j = 0; j < get_value<1>(sub_d); ++j)
    for (auto i = 0; i < get_value<0>(sub_d); ++i)
    {
        auto const true_idx = (i + j*get_value<1>(sub_s)*get_value<0>(sub_d))
                            * get_value<0>(sub_s);

        BOOST_TEST_EQ((sub_l.index(sub_d, i, j)), true_idx);

        BOOST_TEST_EQ(&(sub_l.access(dptr, sub_l.index(sub_d, i, j)))
                    , &(dptr[true_idx])); 

        sub_l.access(dptr, sub_l.index(sub_d, i, j)) = 17;

        BOOST_TEST_EQ((sub_l.access(dptr, sub_l.index(sub_d, i, j))), 17);

        // Bounds-checking.
        BOOST_TEST_EQ((data.at(sub_l.index(sub_d, i, j))), 17);
    }

    // Check final structure. 
    for (auto j = 0; j < get_value<1>(d); ++j)
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        if (  (0 == (i % get_value<0>(sub_s)))
           && (0 == (j % get_value<1>(sub_s)))
           )
        {
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i, j))), 17);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(d, i, j))), 17);
        }
        else
        {
            BOOST_TEST_EQ((l.access(dptr, l.index(d, i, j))), 42);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(d, i, j))), 42);
        }
    }
} // }}}

int main()
{
    // TODO: Test stride == 0.

    // 1D Static
    test_1d_static<1, 1>();
    test_1d_static<1, 30>();

    test_1d_static<2,  30>();
    test_1d_static<5,  30>();
    test_1d_static<10, 30>();
    test_1d_static<30, 30>();

    // 1D Dynamic
    test_1d_dynamic<1, 1>();
    test_1d_dynamic<1, 30>();

    test_1d_dynamic<2,  30>();
    test_1d_dynamic<5,  30>();
    test_1d_dynamic<10, 30>();
    test_1d_dynamic<30, 30>();

    // 2D Static
    test_2d_static<1, 1, 1,  1 >();

    test_2d_static<1, 1, 30, 1 >();
    test_2d_static<1, 1, 1,  30>();
    test_2d_static<1, 1, 30, 30>();

    test_2d_static<2, 1, 30, 1 >();
    test_2d_static<1, 2, 1,  30>();
    test_2d_static<2, 2, 30, 30>();

    test_2d_static<5, 1, 30, 1 >();
    test_2d_static<1, 5, 1,  30>();
    test_2d_static<5, 5, 30, 30>();

    // 2D Dynamic
    test_2d_dynamic<1, 1, 1,  1 >();

    test_2d_dynamic<1, 1, 30, 1 >();
    test_2d_dynamic<1, 1, 1,  30>();
    test_2d_dynamic<1, 1, 30, 30>();

    test_2d_dynamic<2, 1, 30, 1 >();
    test_2d_dynamic<1, 2, 1,  30>();
    test_2d_dynamic<2, 2, 30, 30>();

    test_2d_dynamic<5, 1, 30, 1 >();
    test_2d_dynamic<1, 5, 1,  30>();
    test_2d_dynamic<5, 5, 30, 30>();

    return boost::report_errors();
}

