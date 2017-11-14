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
using std::experimental::layout_mapping_right;

// FIXME FIXME FIXME
//#warning Test stride == 0.
// FIXME FIXME FIXME

template <std::size_t N, std::size_t X>
void test_1d_static()
{ // {{{
    static_assert(0 == (X % N), "X must be divisable by N");

    layout_mapping_right<
        dimensions<X>, dimensions<1>, dimensions<0>
    > const l{};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);

    BOOST_TEST_EQ((l.size()), X);
    BOOST_TEST_EQ((l.span()), X);

    layout_mapping_right<
        dimensions<X / N>, dimensions<N>, dimensions<0>
    > const sub_l{};

    BOOST_TEST_EQ((sub_l.is_regular()), true);

    BOOST_TEST_EQ((sub_l.stride(0)), N);

    BOOST_TEST_EQ((sub_l.size()), X / N);
    BOOST_TEST_EQ((sub_l.span()), X);

    int dptr[X];

    // Set all elements to 42.
    for (auto i = 0; i < l[0]; ++i)
    {
        BOOST_TEST_EQ((l.index(i)), i);

        BOOST_TEST_EQ(&(dptr[l.index(i)]), &(dptr[i]));

        dptr[l.index(i)] = 42;

        BOOST_TEST_EQ((dptr[l.index(i)]), 42);
    }

    // Set every Nth element to 17. 
    for (auto i = 0; i < sub_l[0]; ++i)
    {
        auto const s = sub_l.stepping();
        auto const true_idx = (s[0] * i);

        BOOST_TEST_EQ((sub_l.index(i)), true_idx);

        BOOST_TEST_EQ(&(dptr[sub_l.index(i)]), &(dptr[true_idx])); 

        dptr[sub_l.index(i)] = 17;

        BOOST_TEST_EQ((dptr[sub_l.index(i)]), 17);
    }

    // Check final structure. 
    for (auto i = 0; i < l[0]; ++i)
    {
        BOOST_TEST_EQ((l.index(i)), i);

        // Element not in the strided sub-box.
        if (0 == (i % sub_l.stepping()[0]))
        {
            BOOST_TEST_EQ((dptr[l.index(i)]), 17);
        }
        // Element not in the strided sub-box.
        else
        {
            BOOST_TEST_EQ((dptr[l.index(i)]), 42);
        }
    }
} // }}}

template <std::size_t N, std::size_t X>
void test_1d_dynamic()
{ // {{{
    static_assert(0 == (X % N), "X must be divisable by N");

    layout_mapping_right<
        dimensions<dyn>, dimensions<dyn>, dimensions<0>
    > const l{{X}, {1}, {}};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);

    BOOST_TEST_EQ((l.size()), X);
    BOOST_TEST_EQ((l.span()), X);

    layout_mapping_right<
        dimensions<dyn>, dimensions<dyn>, dimensions<0>
    > const sub_l{{X / N}, {N}, {}};

    BOOST_TEST_EQ((sub_l.is_regular()), true);

    BOOST_TEST_EQ((sub_l.stride(0)), N);

    BOOST_TEST_EQ((sub_l.size()), X / N);
    BOOST_TEST_EQ((sub_l.span()), X);

    // Initialize all elements to 42.
    std::vector<int> data(l[0], 42);
    int* dptr = data.data();

    // Set every Nth element to 17. 
    for (auto i = 0; i < sub_l[0]; ++i)
    {
        auto const s = sub_l.stepping();
        auto const true_idx = (s[0] * i);

        BOOST_TEST_EQ((sub_l.index(i)), true_idx);

        BOOST_TEST_EQ(&(dptr[sub_l.index(i)]), &(dptr[true_idx])); 

        dptr[sub_l.index(i)] = 17;

        BOOST_TEST_EQ((dptr[sub_l.index(i)]), 17);

        // Bounds-checking.
        BOOST_TEST_EQ((data.at(sub_l.index(i))), 17);
    }

    // Check final structure.
    for (auto i = 0; i < l[0]; ++i)
    {
        BOOST_TEST_EQ((l.index(i)), i);

        // Element in the strided sub-box.
        if (0 == (i % sub_l.stepping()[0]))
        {
            BOOST_TEST_EQ((dptr[l.index(i)]), 17);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(i))), 17);
        }
        // Element not in the strided sub-box.
        else
        {
            BOOST_TEST_EQ((dptr[l.index(i)]), 42);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(i))), 42);
        }
    }
} // }}}

template <std::size_t N, std::size_t M, std::size_t X, std::size_t Y>
void test_2d_static()
{ // {{{
    static_assert(0 == (X % N), "X must be divisable by N");
    static_assert(0 == (Y % M), "Y must be divisable by M");

    layout_mapping_right<
        dimensions<X, Y>, dimensions<1, 1>, dimensions<0, 0>
    > const l{};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), X * Y);

    layout_mapping_right<
        dimensions<X / N, Y / M>, dimensions<N, M>, dimensions<0, 0> 
    > const sub_l{};

    BOOST_TEST_EQ((sub_l.is_regular()), true);

    BOOST_TEST_EQ((sub_l.stride(0)), N);
    BOOST_TEST_EQ((sub_l.stride(1)), M);

    BOOST_TEST_EQ((sub_l.size()), (X / N) * (Y / M));
    BOOST_TEST_EQ((sub_l.span()), X * Y);

    int dptr[X * Y];

    // Set all elements to 42.
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (l[1]) * (i) + (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx]));

        dptr[l.index(i, j)] = 42;

        BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);
    }

    // Set every (Nth, Mth) element to 17.
    for (auto j = 0; j < sub_l[1]; ++j)
    for (auto i = 0; i < sub_l[0]; ++i)
    {
        auto const s = sub_l.stepping();
        auto const true_idx = (sub_l[1] * s[1]) * (s[0] * i) + (s[1] * j);

        BOOST_TEST_EQ((sub_l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[sub_l.index(i, j)]), &(dptr[true_idx])); 

        dptr[sub_l.index(i, j)] = 17;

        BOOST_TEST_EQ((dptr[sub_l.index(i, j)]), 17);
    }

    // Check final structure. 
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (l[1]) * (i) + (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        // Element in the strided sub-box.
        if (  (0 == (i % sub_l.stepping()[0]))
           && (0 == (j % sub_l.stepping()[1]))
           )
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 17);
        }
        // Element not in the strided sub-box.
        else
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);
        }
    }
} // }}}

template <std::size_t N, std::size_t M, std::size_t X, std::size_t Y>
void test_2d_dynamic()
{ // {{{
    static_assert(0 == (X % N), "X must be divisable by N");
    static_assert(0 == (Y % M), "Y must be divisable by M");

    layout_mapping_right<
        dimensions<dyn, dyn>, dimensions<dyn, dyn>, dimensions<0, 0>
    > const l{{X, Y}, {1, 1}, {}};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), X * Y);

    layout_mapping_right<
        dimensions<dyn, dyn>, dimensions<dyn, dyn>, dimensions<0, 0> 
    > const sub_l{{X / N, Y / M}, {N, M}, {}};

    BOOST_TEST_EQ((sub_l.is_regular()), true);

    BOOST_TEST_EQ((sub_l.stride(0)), N);
    BOOST_TEST_EQ((sub_l.stride(1)), M);

    BOOST_TEST_EQ((sub_l.size()), (X / N) * (Y / M));
    BOOST_TEST_EQ((sub_l.span()), X * Y);

    // Initialize all elements to 42.
    std::vector<int> data(l[0] * l[1], 42);
    int* dptr = data.data();

    // Set every (Nth, Mth) element to 17.
    for (auto j = 0; j < sub_l[1]; ++j)
    for (auto i = 0; i < sub_l[0]; ++i)
    {
        auto const s = sub_l.stepping();
        auto const true_idx = (sub_l[1] * s[1]) * (s[0] * i) + (s[1] * j);

        BOOST_TEST_EQ((sub_l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[sub_l.index(i, j)]), &(dptr[true_idx])); 

        dptr[sub_l.index(i, j)] = 17;

        BOOST_TEST_EQ((dptr[sub_l.index(i, j)]), 17);

        // Bounds-checking.
        BOOST_TEST_EQ((data.at(sub_l.index(i, j))), 17);
    }

    // Check final structure. 
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (l[1]) * (i) + (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        // Element in the strided sub-box.
        if (  (0 == (i % sub_l.stepping()[0]))
           && (0 == (j % sub_l.stepping()[1]))
           )
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 17);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(i, j))), 17);
        }
        // Element not in the strided sub-box.
        else
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(i, j))), 42);
        }
    }
} // }}}

int main()
{
    ///////////////////////////////////////////////////////////////////////////
    // 1D Static

    test_1d_static<1, 1>();
    test_1d_static<1, 30>();

    test_1d_static<2,  30>();
    test_1d_static<5,  30>();
    test_1d_static<10, 30>();
    test_1d_static<30, 30>();

    ///////////////////////////////////////////////////////////////////////////
    // 1D Dynamic

    test_1d_dynamic<1, 1>();
    test_1d_dynamic<1, 30>();

    test_1d_dynamic<2,  30>();
    test_1d_dynamic<5,  30>();
    test_1d_dynamic<10, 30>();
    test_1d_dynamic<30, 30>();

    ///////////////////////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////////////////////
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

