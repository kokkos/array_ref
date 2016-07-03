////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <vector>
#include <tuple>

#include <array_ref>

using std::vector;
using std::tuple;

using std::experimental::dyn;
using std::experimental::dimensions;
using std::experimental::basic_layout_left;

// FIXME FIXME FIXME
#warning Verify size()/span() tests provide sufficient coverage. 
// FIXME FIXME FIXME

template <std::size_t N, std::size_t M, std::size_t X, std::size_t Y>
void test_2d_static()
{ // {{{
    dimensions<X, Y> d;
    dimensions<N, M> p;

    basic_layout_left<dimensions<1, 1>, decltype(p)> const l;

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((d.size()),  d[0] * d[1]);
    BOOST_TEST_EQ((l.span(d)), (d[0] + p[0]) * (d[1] + p[1]));

    int dptr[(d[0] + p[0]) * (d[1] + p[1])];

    // Set all real elements to 42.
    for (auto j = 0; j < d[1]; ++j)
    for (auto i = 0; i < d[0]; ++i)
    {
        auto const true_idx = (i) + (d[0] + p[0]) * (j);

        BOOST_TEST_EQ((l.index(d, i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(d, i, j)]), &(dptr[true_idx]));

        dptr[l.index(d, i, j)] = 42;

        BOOST_TEST_EQ((dptr[true_idx]),         42);
        BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 42);
    }

    // Set X pad elements to 17 and Y pad elements to 24. 
    for (auto j = 0; j < d[1] + p[1]; ++j)
    for (auto i = 0; i < d[0] + p[0]; ++i)
    {
        auto const true_idx = (i) + (d[0] + p[0]) * (j);

        BOOST_TEST_EQ((l.index(d, i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(d, i, j)]), &(dptr[true_idx])); 

        // X-pad element.
        if      ((d[0] <= i) && (i < (d[0] + p[0])))
        {
            dptr[l.index(d, i, j)] = 17;
            
            BOOST_TEST_EQ((dptr[true_idx]),         17);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 17);
        }

        // Y-pad element.
        else if ((d[1] <= j) && (j < (d[1] + p[1])))
        {
            dptr[l.index(d, i, j)] = 24;

            BOOST_TEST_EQ((dptr[true_idx]),         24);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 24);
        }
    }

    // Check final structure. 
    for (auto j = 0; j < d[1] + p[1]; ++j)
    for (auto i = 0; i < d[0] + p[0]; ++i)
    {
        auto const true_idx = (i) + (d[0] + p[0]) * (j);

        BOOST_TEST_EQ((l.index(d, i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(d, i, j)]), &(dptr[true_idx])); 

        // X-pad element.
        if      ((d[0] <= i) && (i < (d[0] + p[0])))
        {
            BOOST_TEST_EQ((dptr[true_idx]),         17);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 17);
        }

        // Y-pad element.
        else if ((d[1] <= j) && (j < (d[1] + p[1])))
        {
            BOOST_TEST_EQ((dptr[true_idx]),         24);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 24);
        }

        // Real element.
        else
        {
            BOOST_TEST_EQ((dptr[true_idx]),         42);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 42);
        }
    }
} // }}}

template <std::size_t N, std::size_t M, std::size_t X, std::size_t Y>
void test_2d_dynamic()
{ // {{{
    dimensions<dyn, dyn> d(X, Y);
    dimensions<dyn, dyn> p(N, M);

    basic_layout_left<dimensions<1, 1>, decltype(p)> const
        l(dimensions<1, 1>(), p);

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((d.size()),  d[0] * d[1]);
    BOOST_TEST_EQ((l.span(d)), (d[0] + p[0]) * (d[1] + p[1]));

    // Initialize all elements to 42.
    std::vector<int> data((d[0] + p[0]) * (d[1] + p[1]), 42);
    int* dptr = data.data();

    // Set X pad elements to 17 and Y pad elements to 24. 
    for (auto j = 0; j < d[1] + p[1]; ++j)
    for (auto i = 0; i < d[0] + p[0]; ++i)
    {
        auto const true_idx = (i) + (d[0] + p[0]) * (j);

        // X-pad element.
        if      ((d[0] <= i) && (i < (d[0] + p[0])))
        {
            dptr[l.index(d, i, j)] = 17;
            
            BOOST_TEST_EQ((dptr[true_idx]),         17);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 17);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(true_idx)),         17);
            BOOST_TEST_EQ((data.at(l.index(d, i, j))), 17);
        }

        // Y-pad element.
        else if ((d[1] <= j) && (j < (d[1] + p[1])))
        {
            dptr[l.index(d, i, j)] = 24;

            BOOST_TEST_EQ((dptr[true_idx]),         24);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 24);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(true_idx)),         24);
            BOOST_TEST_EQ((data.at(l.index(d, i, j))), 24);
        }
    }

    // Check final structure. 
    for (auto j = 0; j < d[1] + p[1]; ++j)
    for (auto i = 0; i < d[0] + p[0]; ++i)
    {
        auto const true_idx = i + j * (d[0] + N);

        // X-pad element.
        if      ((d[0] <= i) && (i < (d[0] + p[0])))
        {
            BOOST_TEST_EQ((dptr[true_idx]),         17);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 17);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(true_idx)),         17);
            BOOST_TEST_EQ((data.at(l.index(d, i, j))), 17);
        }

        // Y-pad element.
        else if ((d[1] <= j) && (j < (d[1] + p[1])))
        {
            BOOST_TEST_EQ((dptr[true_idx]),         24);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 24);

            // Bounds-checking.
            BOOST_TEST_EQ((data.at(true_idx)),         24);
            BOOST_TEST_EQ((data.at(l.index(d, i, j))), 24);
        }

        // Real element.
        else
        {
            BOOST_TEST_EQ((dptr[true_idx]),         42);
            BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 42);
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

