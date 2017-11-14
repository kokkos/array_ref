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

template <std::size_t N, std::size_t M, std::size_t X, std::size_t Y>
void test_2d_static()
{ // {{{
    layout_mapping_left<
        dimensions<X, Y>, dimensions<1, 1>, dimensions<N, M>
    > const l{};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), (X + N) * (Y + M));

    int dptr[(X + N) * (Y + M)];

    // Set all real elements to 42.
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (i) + (l[0] + l.padding()[0]) * (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx]));

        dptr[l.index(i, j)] = 42;

        BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);
    }

    // Set X pad elements to 17 and Y pad elements to 24. 
    for (auto j = 0; j < l[1] + l.padding()[1]; ++j)
    for (auto i = 0; i < l[0] + l.padding()[0]; ++i)
    {
        auto const true_idx = (i) + (l[0] + l.padding()[0]) * (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx])); 

        // X-pad element.
        if      ((l[0] <= i) && (i < (l[0] + l.padding()[0])))
        {
            dptr[l.index(i, j)] = 17;
            
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 17);
        }

        // Y-pad element.
        else if ((l[1] <= j) && (j < (l[1] + l.padding()[1])))
        {
            dptr[l.index(i, j)] = 24;

            BOOST_TEST_EQ((dptr[l.index(i, j)]), 24);
        }
    }

    // Check final structure. 
    for (auto j = 0; j < l[1] + l.padding()[1]; ++j)
    for (auto i = 0; i < l[0] + l.padding()[0]; ++i)
    {
        auto const true_idx = (i) + (l[0] + l.padding()[0]) * (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx])); 

        // X-pad element.
        if      ((l[0] <= i) && (i < (l[0] + l.padding()[0])))
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 17);
        }

        // Y-pad element.
        else if ((l[1] <= j) && (j < (l[1] + l.padding()[1])))
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 24);
        }

        // Real element.
        else
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);
        }
    }
} // }}}

template <std::size_t N, std::size_t M, std::size_t X, std::size_t Y>
void test_2d_dynamic()
{ // {{{
    layout_mapping_left<
        dimensions<dyn, dyn>, dimensions<1, 1>, dimensions<dyn, dyn>
    > const l{{X, Y}, {}, {N, M}};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), (X + N) * (Y + M));

    // Initialize all elements to 42.
    std::vector<int> data(
        (l[0] + l.padding()[0]) * (l[1] + l.padding()[1]), 42
    );
    int* dptr = data.data();

    // Set X pad elements to 17 and Y pad elements to 24. 
    for (auto j = 0; j < l[1] + l.padding()[1]; ++j)
    for (auto i = 0; i < l[0] + l.padding()[0]; ++i)
    {
        auto const true_idx = (i) + (l[0] + l.padding()[0]) * (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        // X-pad element.
        if      ((l[0] <= i) && (i < (l[0] + l.padding()[0])))
        {
            dptr[l.index(i, j)] = 17;
            
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 17);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(i, j))), 17);
        }

        // Y-pad element.
        else if ((l[1] <= j) && (j < (l[1] + l.padding()[1])))
        {
            dptr[l.index(i, j)] = 24;

            BOOST_TEST_EQ((dptr[l.index(i, j)]), 24);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(i, j))), 24);
        }
    }

    // Check final structure. 
    for (auto j = 0; j < l[1] + l.padding()[1]; ++j)
    for (auto i = 0; i < l[0] + l.padding()[0]; ++i)
    {
        auto const true_idx = i + j * (l[0] + N);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        // X-pad element.
        if      ((l[0] <= i) && (i < (l[0] + l.padding()[0])))
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 17);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(i, j))), 17);
        }

        // Y-pad element.
        else if ((l[1] <= j) && (j < (l[1] + l.padding()[1])))
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 24);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(l.index(i, j))), 24);
        }

        // Real element.
        else
        {
            BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);
        }
    }
} // }}}

int main()
{
    ///////////////////////////////////////////////////////////////////////////
    // 2D Static

    test_2d_static<0, 0, 1,  1 >();

    test_2d_static<0, 0, 30, 1 >();
    test_2d_static<0, 0, 1,  30>();
    test_2d_static<0, 0, 30, 30>();

    test_2d_static<1, 0, 30, 1 >();
    test_2d_static<0, 1, 1,  30>();
    test_2d_static<1, 1, 30, 30>();

    test_2d_static<5, 0, 30, 1 >();
    test_2d_static<0, 5, 1,  30>();
    test_2d_static<5, 5, 30, 30>();

    ///////////////////////////////////////////////////////////////////////////
    // 2D Dynamic

    test_2d_dynamic<0, 0, 1,  1 >();

    test_2d_dynamic<0, 0, 30, 1 >();
    test_2d_dynamic<0, 0, 1,  30>();
    test_2d_dynamic<0, 0, 30, 30>();

    test_2d_dynamic<1, 0, 30, 1 >();
    test_2d_dynamic<0, 1, 1,  30>();
    test_2d_dynamic<1, 1, 30, 30>();

    test_2d_dynamic<5, 0, 30, 1 >();
    test_2d_dynamic<0, 5, 1,  30>();
    test_2d_dynamic<5, 5, 30, 30>();

    return boost::report_errors();
}

