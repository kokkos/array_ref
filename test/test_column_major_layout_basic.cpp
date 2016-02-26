////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <vector>

#include <array_ref>

using std::experimental::dimensions;
using std::experimental::get_value;
using std::experimental::column_major_layout;
constexpr auto dyn = std::experimental::dynamic_dimension;

template <std::size_t X>
void test_1d_static()
{ // {{{
    dimensions<X> d;
    column_major_layout<int, dimensions<1>, dimensions<0> > l;

    int dptr[X];

    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        BOOST_TEST_EQ((l.index(d, i)), i);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i))), &(dptr[i])); 

        l.access(dptr, l.index(d, i)) = i;

        BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), i); 
    }
} // }}}

template <std::size_t X>
void test_1d_dynamic()
{ // {{{
    dimensions<dyn> d(X);
    column_major_layout<int, dimensions<1>, dimensions<0> > l;

    std::vector<int> data(get_value<0>(d), 42);
    int* dptr = data.data();

    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        BOOST_TEST_EQ((l.index(d, i)), i);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i))), &(dptr[i])); 

        BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 42); 

        l.access(dptr, l.index(d, i)) = i;

        BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), i); 

        // Bound-checking.
        BOOST_TEST_EQ((data.at(l.index(d, i))), i); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_static()
{ // {{{
    dimensions<X, Y> d;
    column_major_layout<
        std::tuple<int, int>, dimensions<1, 1>, dimensions<0, 0>
    > l;

    std::tuple<int, int> dptr[X*Y];

    for (auto j = 0; j < get_value<1>(d); ++j)
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        auto const true_idx = i + j*get_value<0>(d);

        BOOST_TEST_EQ((l.index(d, i, j)), true_idx);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i, j))), &(dptr[true_idx])); 

        std::get<0>(l.access(dptr, l.index(d, i, j))) = i;
        std::get<1>(l.access(dptr, l.index(d, i, j))) = j;

        BOOST_TEST_EQ((std::get<0>(l.access(dptr, l.index(d, i, j)))), i); 
        BOOST_TEST_EQ((std::get<1>(l.access(dptr, l.index(d, i, j)))), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_dynamic()
{ // {{{
    dimensions<dyn, dyn> d(X, Y);
    column_major_layout<
        std::tuple<int, int>, dimensions<1, 1>, dimensions<0, 0>
    > l;

    std::vector<std::tuple<int, int> > data(
        get_value<0>(d)*get_value<1>(d), std::tuple<int, int>(17, 42)
    );

    std::tuple<int, int>* dptr = data.data();

    for (auto j = 0; j < get_value<1>(d); ++j)
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        auto const true_idx = i + j*get_value<0>(d);

        BOOST_TEST_EQ((l.index(d, i, j)), true_idx);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i, j))), &(dptr[true_idx])); 

        BOOST_TEST_EQ((std::get<0>(l.access(dptr, l.index(d, i, j)))), 17); 
        BOOST_TEST_EQ((std::get<1>(l.access(dptr, l.index(d, i, j)))), 42); 

        std::get<0>(l.access(dptr, l.index(d, i, j))) = i;
        std::get<1>(l.access(dptr, l.index(d, i, j))) = j;

        BOOST_TEST_EQ((std::get<0>(l.access(dptr, l.index(d, i, j)))), i); 
        BOOST_TEST_EQ((std::get<1>(l.access(dptr, l.index(d, i, j)))), j); 

        // Bound-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(l.index(d, i, j)))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(l.index(d, i, j)))), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_mixed()
{ // {{{
    dimensions<dyn, Y> d(X);
    column_major_layout<
        std::tuple<int, int>, dimensions<1, 1>, dimensions<0, 0>
    > l;

    std::vector<std::tuple<int, int> > data(
        get_value<0>(d)*get_value<1>(d), std::tuple<int, int>(17, 42)
    );

    std::tuple<int, int>* dptr = data.data();

    for (auto j = 0; j < get_value<1>(d); ++j)
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        auto const true_idx = i + j*get_value<0>(d);

        BOOST_TEST_EQ((l.index(d, i, j)), true_idx);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i, j))), &(dptr[true_idx])); 

        BOOST_TEST_EQ((std::get<0>(l.access(dptr, l.index(d, i, j)))), 17); 
        BOOST_TEST_EQ((std::get<1>(l.access(dptr, l.index(d, i, j)))), 42); 

        std::get<0>(l.access(dptr, l.index(d, i, j))) = i;
        std::get<1>(l.access(dptr, l.index(d, i, j))) = j;

        BOOST_TEST_EQ((std::get<0>(l.access(dptr, l.index(d, i, j)))), i); 
        BOOST_TEST_EQ((std::get<1>(l.access(dptr, l.index(d, i, j)))), j); 

        // Bound-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(l.index(d, i, j)))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(l.index(d, i, j)))), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y, std::size_t Z>
void test_3d_static()
{ // {{{
    dimensions<X, Y, Z> d;
    column_major_layout<
        std::tuple<int, int, int>, dimensions<1, 1, 1>, dimensions<0, 0, 0>
    > l;

    std::tuple<int, int, int> dptr[X*Y*Z];

    for (auto k = 0; k < get_value<2>(d); ++k)
    for (auto j = 0; j < get_value<1>(d); ++j)
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        auto const true_idx =
            i + j*get_value<0>(d) + k*get_value<1>(d)*get_value<0>(d);

        BOOST_TEST_EQ((l.index(d, i, j, k)), true_idx);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i, j, k))), &(dptr[true_idx])); 

        std::get<0>(l.access(dptr, l.index(d, i, j, k))) = i;
        std::get<1>(l.access(dptr, l.index(d, i, j, k))) = j;
        std::get<2>(l.access(dptr, l.index(d, i, j, k))) = k;

        BOOST_TEST_EQ((std::get<0>(l.access(dptr, l.index(d, i, j, k)))), i); 
        BOOST_TEST_EQ((std::get<1>(l.access(dptr, l.index(d, i, j, k)))), j); 
        BOOST_TEST_EQ((std::get<2>(l.access(dptr, l.index(d, i, j, k)))), k); 
    }
} // }}}

template <std::size_t X, std::size_t Y, std::size_t Z>
void test_3d_dynamic()
{ // {{{
    dimensions<dyn, dyn, dyn> d(X, Y, Z);
    column_major_layout<
        std::tuple<int, int, int>, dimensions<1, 1, 1>, dimensions<0, 0, 0>
    > l;

    std::vector<std::tuple<int, int, int> > data(
        get_value<0>(d)*get_value<1>(d)*get_value<2>(d)
      , std::tuple<int, int, int>(17, 42, 73)
    );

    std::tuple<int, int, int>* dptr = data.data();

    for (auto k = 0; k < get_value<2>(d); ++k)
    for (auto j = 0; j < get_value<1>(d); ++j)
    for (auto i = 0; i < get_value<0>(d); ++i)
    {
        auto const true_idx =
            i + j*get_value<0>(d) + k*get_value<1>(d)*get_value<0>(d);

        BOOST_TEST_EQ((l.index(d, i, j, k)), true_idx);

        BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i, j, k))), &(dptr[true_idx])); 

        BOOST_TEST_EQ((std::get<0>(l.access(dptr, l.index(d, i, j, k)))), 17); 
        BOOST_TEST_EQ((std::get<1>(l.access(dptr, l.index(d, i, j, k)))), 42); 
        BOOST_TEST_EQ((std::get<2>(l.access(dptr, l.index(d, i, j, k)))), 73); 

        std::get<0>(l.access(dptr, l.index(d, i, j, k))) = i;
        std::get<1>(l.access(dptr, l.index(d, i, j, k))) = j;
        std::get<2>(l.access(dptr, l.index(d, i, j, k))) = k;

        BOOST_TEST_EQ((std::get<0>(l.access(dptr, l.index(d, i, j, k)))), i); 
        BOOST_TEST_EQ((std::get<1>(l.access(dptr, l.index(d, i, j, k)))), j); 
        BOOST_TEST_EQ((std::get<2>(l.access(dptr, l.index(d, i, j, k)))), k); 

        // Bound-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(l.index(d, i, j, k)))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(l.index(d, i, j, k)))), j); 
        BOOST_TEST_EQ((std::get<2>(data.at(l.index(d, i, j, k)))), k); 
    }
} // }}}

int main()
{
    {
        dimensions<> d;
        column_major_layout<int, dimensions<>, dimensions<> > l;

        int data = 42;
        int* dptr = &data;

        BOOST_TEST_EQ((l.index(d)), 0);

        BOOST_TEST_EQ((l.access(dptr, l.index(d))), 42); 
        BOOST_TEST_EQ(&(l.access(dptr, l.index(d))), dptr); 

        l.access(dptr, l.index(d)) = 17;

        BOOST_TEST_EQ((l.access(dptr, l.index(d))), 17); 
    }

    // 1D Static
    test_1d_static<1 >();
    test_1d_static<30>();

    // 1D Dynamic
    test_1d_dynamic<1 >();
    test_1d_dynamic<30>();

    // 2D Static
    test_2d_static<1,  1 >();

    test_2d_static<30, 1 >();
    test_2d_static<1,  30>();

    test_2d_static<30, 15>();
    test_2d_static<15, 30>();

    test_2d_static<30, 30>();

    // 2D Dynamic
    test_2d_dynamic<1,  1 >();

    test_2d_dynamic<30, 1 >();
    test_2d_dynamic<1,  30>();

    test_2d_dynamic<30, 15>();
    test_2d_dynamic<15, 30>();

    test_2d_dynamic<30, 30>();

    // 2D Mixed
    test_2d_mixed<1,  1 >();

    test_2d_mixed<30, 1 >();
    test_2d_mixed<1,  30>();

    test_2d_mixed<30, 15>();
    test_2d_mixed<15, 30>();

    test_2d_mixed<30, 30>();

    // 3D Static
    test_3d_static<1,  1,  1 >();

    test_3d_static<30, 1,  1 >();
    test_3d_static<1,  30, 1 >();
    test_3d_static<1,  1,  30>();

    test_3d_static<1,  30, 30>();
    test_3d_static<30, 1,  30>();
    test_3d_static<30, 30, 1 >();

    test_3d_static<30, 15, 15>();
    test_3d_static<15, 30, 15>();
    test_3d_static<15, 15, 30>();

    test_3d_static<15, 30, 30>();
    test_3d_static<30, 15, 30>();
    test_3d_static<30, 30, 15>();

    test_3d_static<30, 30, 30>();

    // 3D Dynamic
    test_3d_dynamic<1,  1,  1 >();

    test_3d_dynamic<30, 1,  1 >();
    test_3d_dynamic<1,  30, 1 >();
    test_3d_dynamic<1,  1,  30>();

    test_3d_dynamic<1,  30, 30>();
    test_3d_dynamic<30, 1,  30>();
    test_3d_dynamic<30, 30, 1 >();

    test_3d_dynamic<30, 15, 15>();
    test_3d_dynamic<15, 30, 15>();
    test_3d_dynamic<15, 15, 30>();

    test_3d_dynamic<15, 30, 30>();
    test_3d_dynamic<30, 15, 30>();
    test_3d_dynamic<30, 30, 15>();

    test_3d_dynamic<30, 30, 30>();

    return boost::report_errors();
}
