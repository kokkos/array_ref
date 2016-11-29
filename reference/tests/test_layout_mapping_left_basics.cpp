////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <vector>
#include <tuple>

#include <mdspan>

using std::vector;
using std::tuple;

using std::experimental::dyn;
using std::experimental::dimensions;
using std::experimental::layout_mapping_left;

template <std::size_t X>
void test_1d_static()
{ // {{{
    layout_mapping_left<
        dimensions<X>, dimensions<1>, dimensions<0>
    > const l{};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);

    BOOST_TEST_EQ((l.size()), X);
    BOOST_TEST_EQ((l.span()), X);

    int dptr[X];

    // Set all elements to a unique value.
    for (auto i = 0; i < l[0]; ++i)
    {
        BOOST_TEST_EQ((l.index(i)), i);

        BOOST_TEST_EQ(&(dptr[l.index(i)]), &(dptr[i])); 

        dptr[l.index(i)] = i;

        BOOST_TEST_EQ((dptr[l.index(i)]), i); 
    }
} // }}}

template <std::size_t X>
void test_1d_dynamic()
{ // {{{
    layout_mapping_left<
        dimensions<dyn>, dimensions<1>, dimensions<0>
    > const l{{X}};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);

    BOOST_TEST_EQ((l.size()), X);
    BOOST_TEST_EQ((l.span()), X);

    vector<int> data(l[0]);
    int* dptr = data.data();

    // Set all elements to a unique value.
    for (auto i = 0; i < l[0]; ++i)
    {
        BOOST_TEST_EQ((l.index(i)), i);

        BOOST_TEST_EQ(&(dptr[l.index(i)]), &(dptr[i])); 

        dptr[l.index(i)] = i;

        BOOST_TEST_EQ((dptr[l.index(i)]), i); 

        // Bounds-checking.
        BOOST_TEST_EQ((data.at(l.index(i))), i); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_static()
{ // {{{
    layout_mapping_left<
        dimensions<X, Y>, dimensions<1, 1>, dimensions<0, 0>
    > const l{};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), X * Y);

    tuple<int, int> dptr[X * Y];

    // Set all elements to a unique value.
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (i) + (l[0]) * (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx])); 

        std::get<0>(dptr[l.index(i, j)]) = i;
        std::get<1>(dptr[l.index(i, j)]) = j;

        BOOST_TEST_EQ((std::get<0>(dptr[l.index(i, j)])), i); 
        BOOST_TEST_EQ((std::get<1>(dptr[l.index(i, j)])), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_dynamic()
{ // {{{
    layout_mapping_left<
        dimensions<dyn, dyn>, dimensions<1, 1>, dimensions<0, 0>
    > const l{{X, Y}};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), X * Y);

    vector<tuple<int, int> > data(l[0] * l[1]);
    tuple<int, int>* dptr = data.data();

    // Set all elements to a unique value.
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (i) + (l[0]) * (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx])); 

        std::get<0>(dptr[l.index(i, j)]) = i;
        std::get<1>(dptr[l.index(i, j)]) = j;

        BOOST_TEST_EQ((std::get<0>(dptr[l.index(i, j)])), i); 
        BOOST_TEST_EQ((std::get<1>(dptr[l.index(i, j)])), j); 

        // Bounds-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(l.index(i, j)))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(l.index(i, j)))), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_mixed()
{ // {{{
    layout_mapping_left<
        dimensions<dyn, Y>, dimensions<1, 1>, dimensions<0, 0>
    > const l{{X}};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), X * Y);

    vector<tuple<int, int> > data(l[0] * l[1]);
    tuple<int, int>* dptr = data.data();

    // Set all elements to a unique value.
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (i) + (l[0]) * (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx])); 

        std::get<0>(dptr[l.index(i, j)]) = i;
        std::get<1>(dptr[l.index(i, j)]) = j;

        BOOST_TEST_EQ((std::get<0>(dptr[l.index(i, j)])), i); 
        BOOST_TEST_EQ((std::get<1>(dptr[l.index(i, j)])), j); 

        // Bounds-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(l.index(i, j)))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(l.index(i, j)))), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y, std::size_t Z>
void test_3d_static()
{ // {{{
    layout_mapping_left<
        dimensions<X, Y, Z>, dimensions<1, 1, 1>, dimensions<0, 0, 0>
    >  const l{};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);
    BOOST_TEST_EQ((l.stride(2)), 1);

    BOOST_TEST_EQ((l.size()), X * Y * Z);
    BOOST_TEST_EQ((l.span()), X * Y * Z);

    tuple<int, int, int> dptr[X * Y * Z];

    // Set all elements to a unique value.
    for (auto k = 0; k < l[2]; ++k)
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (i) + (l[0]) * (j) + (l[0]) * (l[1]) * (k);

        BOOST_TEST_EQ((l.index(i, j, k)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j, k)]), &(dptr[true_idx])); 

        std::get<0>(dptr[l.index(i, j, k)]) = i;
        std::get<1>(dptr[l.index(i, j, k)]) = j;
        std::get<2>(dptr[l.index(i, j, k)]) = k;

        BOOST_TEST_EQ((std::get<0>(dptr[l.index(i, j, k)])), i); 
        BOOST_TEST_EQ((std::get<1>(dptr[l.index(i, j, k)])), j); 
        BOOST_TEST_EQ((std::get<2>(dptr[l.index(i, j, k)])), k); 
    }
} // }}}

template <std::size_t X, std::size_t Y, std::size_t Z>
void test_3d_dynamic()
{ // {{{
    layout_mapping_left<
        dimensions<dyn, dyn, dyn>, dimensions<1, 1, 1>, dimensions<0, 0, 0>
    > const l{{X, Y, Z}};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);
    BOOST_TEST_EQ((l.stride(2)), 1);

    BOOST_TEST_EQ((l.size()), l[0] * l[1] * l[2]);
    BOOST_TEST_EQ((l.span()), l[0] * l[1] * l[2]);

    vector<tuple<int, int, int> > data(l[0] * l[1] * l[2]);
    tuple<int, int, int>* dptr = data.data();

    // Set all elements to a unique value.
    for (auto k = 0; k < l[2]; ++k)
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (i) + (l[0]) * (j) + (l[0]) * (l[1]) * (k);

        BOOST_TEST_EQ((l.index(i, j, k)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j, k)]), &(dptr[true_idx])); 

        std::get<0>(dptr[l.index(i, j, k)]) = i;
        std::get<1>(dptr[l.index(i, j, k)]) = j;
        std::get<2>(dptr[l.index(i, j, k)]) = k;

        BOOST_TEST_EQ((std::get<0>(dptr[l.index(i, j, k)])), i); 
        BOOST_TEST_EQ((std::get<1>(dptr[l.index(i, j, k)])), j); 
        BOOST_TEST_EQ((std::get<2>(dptr[l.index(i, j, k)])), k); 

        // Bounds-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(l.index(i, j, k)))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(l.index(i, j, k)))), j); 
        BOOST_TEST_EQ((std::get<2>(data.at(l.index(i, j, k)))), k); 
    }
} // }}}

int main()
{
    // Empty
    {
        layout_mapping_left<dimensions<>, dimensions<>, dimensions<> > const l;

        BOOST_TEST_EQ((l.is_regular()), true);

        BOOST_TEST_EQ((l.size()), 1);
        BOOST_TEST_EQ((l.span()), 1);

        int data = 42;
        int* dptr = &data;

        BOOST_TEST_EQ((l.index()), 0);

        BOOST_TEST_EQ((dptr[l.index()]), 42); 

        BOOST_TEST_EQ(&(dptr[l.index()]), dptr); 

        dptr[l.index()] = 17;

        BOOST_TEST_EQ((dptr[l.index()]), 17); 
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
