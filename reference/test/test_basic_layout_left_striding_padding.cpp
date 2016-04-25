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

template <
    std::size_t StrideX, std::size_t StrideY 
  , std::size_t PadX, std::size_t PadY 
  , std::size_t X, std::size_t Y
    >
void test_2d_static()
{ // {{{
    static_assert(0 == (X % StrideX), "X must be divisable by StrideX");
    static_assert(0 == (Y % StrideY), "Y must be divisable by StrideY");

    dimensions<X,    Y   > d;
    dimensions<1,    1   > s;
    dimensions<PadX, PadY> p;

    dimensions<X / StrideX, Y / StrideY> sub_d;
    dimensions<StrideX,     StrideY    > sub_s;

    basic_layout_left<decltype(s), decltype(p)> const l;

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((d.size()),  d[0] * d[1]);
    BOOST_TEST_EQ((l.span(d)), (d[0] + p[0]) * (d[1] + p[1]));

    basic_layout_left<decltype(sub_s), decltype(p)> const sub_l;

    BOOST_TEST_EQ((sub_l.is_regular()), true);

    BOOST_TEST_EQ((sub_l.stride(0)), sub_s[0]);
    BOOST_TEST_EQ((sub_l.stride(1)), sub_s[1]);

    BOOST_TEST_EQ((sub_d.size()),      (d[0] / sub_s[0]) * (d[1] / sub_s[1]));
    BOOST_TEST_EQ((sub_l.span(sub_d)), (d[0] + p[0]) * (d[1] + p[1]));

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

    // Set every (StrideXth, StrideYth) element to 71.
    for (auto j = 0; j < sub_d[1]; ++j)
    for (auto i = 0; i < sub_d[0]; ++i)
    {
        auto const true_idx
            = (sub_s[0] * i) + (sub_d[0] * sub_s[0] + p[0]) * (sub_s[1] * j);

        BOOST_TEST_EQ((sub_l.index(sub_d, i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[sub_l.index(sub_d, i, j)]), &(dptr[true_idx])); 

        dptr[sub_l.index(sub_d, i, j)] = 71;

        BOOST_TEST_EQ((dptr[true_idx]),                 71);
        BOOST_TEST_EQ((dptr[sub_l.index(sub_d, i, j)]), 71);
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
            // Real element in the strided sub-box.
            if (  (0 == (i % sub_s[0]))
               && (0 == (j % sub_s[1]))
               )
            {
                BOOST_TEST_EQ((dptr[true_idx]),         71);
                BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 71);
            }
            // Real element not in the strided sub-box.
            else
            {
                BOOST_TEST_EQ((dptr[true_idx]),         42);
                BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 42);
            }
        }
    }
} // }}}

template <
    std::size_t StrideX, std::size_t StrideY 
  , std::size_t PadX, std::size_t PadY 
  , std::size_t X, std::size_t Y
    >
void test_2d_dynamic()
{ // {{{
    dimensions<dyn, dyn> d(X,    Y   );
    dimensions<dyn, dyn> s(1,    1   );
    dimensions<dyn, dyn> p(PadX, PadY);

    dimensions<dyn, dyn> sub_d(X / StrideX, Y / StrideY);
    dimensions<dyn, dyn> sub_s(StrideX,     StrideY    );

    basic_layout_left<decltype(s), decltype(p)> const l(s, p);

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((d.size()),  d[0] * d[1]);
    BOOST_TEST_EQ((l.span(d)), (d[0] + p[0]) * (d[1] + p[1]));

    basic_layout_left<decltype(sub_s), decltype(p)> const sub_l(sub_s, p);

    BOOST_TEST_EQ((sub_l.is_regular()), true);

    BOOST_TEST_EQ((sub_l.stride(0)), sub_s[0]);
    BOOST_TEST_EQ((sub_l.stride(1)), sub_s[1]);

    BOOST_TEST_EQ((sub_d.size()),      (d[0] / sub_s[0]) * (d[1] / sub_s[1]));
    BOOST_TEST_EQ((sub_l.span(sub_d)), (d[0] + p[0]) * (d[1] + p[1]));

    // Initialize all elements as 42.
    std::vector<int> data((d[0] + p[0]) * (d[1] + p[1]), 42);
    int* dptr = data.data();

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

    // Set every (StrideXth, StrideYth) element to 71.
    for (auto j = 0; j < sub_d[1]; ++j)
    for (auto i = 0; i < sub_d[0]; ++i)
    {
        auto const true_idx
            = (sub_s[0] * i) + (sub_d[0] * sub_s[0] + p[0]) * (sub_s[1] * j);

        BOOST_TEST_EQ((sub_l.index(sub_d, i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[sub_l.index(sub_d, i, j)]), &(dptr[true_idx])); 

        dptr[sub_l.index(sub_d, i, j)] = 71;

        BOOST_TEST_EQ((dptr[true_idx]),                 71);
        BOOST_TEST_EQ((dptr[sub_l.index(sub_d, i, j)]), 71);

        // Bounds-checking.
        BOOST_TEST_EQ((data.at(true_idx)),                 71);
        BOOST_TEST_EQ((data.at(sub_l.index(sub_d, i, j))), 71);
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
            // Real element in the strided sub-box.
            if (  (0 == (i % sub_s[0]))
               && (0 == (j % sub_s[1]))
               )
            {
                BOOST_TEST_EQ((dptr[true_idx]),         71);
                BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 71);

                // Bounds-checking.
                BOOST_TEST_EQ((data.at(true_idx)),         71);
                BOOST_TEST_EQ((data.at(l.index(d, i, j))), 71);
            }
            // Real element not in the strided sub-box.
            else
            {
                BOOST_TEST_EQ((dptr[true_idx]),         42);
                BOOST_TEST_EQ((dptr[l.index(d, i, j)]), 42);

                // Bounds-checking.
                BOOST_TEST_EQ((data.at(true_idx)),         42);
                BOOST_TEST_EQ((data.at(l.index(d, i, j))), 42);
            }
        }
    }
} // }}}

int main()
{
    ///////////////////////////////////////////////////////////////////////////
    // 2D Static

    //             Striding   Padding   Dimensions
    test_2d_static<5, 1,      1, 0,     30, 30>();
    test_2d_static<1, 5,      1, 0,     30, 30>();
    test_2d_static<5, 5,      1, 0,     30, 30>();
    test_2d_static<5, 1,      0, 1,     30, 30>();
    test_2d_static<1, 5,      0, 1,     30, 30>();
    test_2d_static<5, 5,      0, 1,     30, 30>();
    test_2d_static<5, 1,      1, 0,     30, 30>();
    test_2d_static<1, 5,      1, 0,     30, 30>();
    test_2d_static<5, 5,      1, 0,     30, 30>();
    test_2d_static<5, 1,      1, 1,     30, 30>();
    test_2d_static<1, 5,      1, 1,     30, 30>();

    test_2d_static<5, 5,      1, 1,     30, 30>();
    test_2d_static<5, 1,      1, 0,     30, 30>();
    test_2d_static<1, 5,      1, 0,     30, 30>();
    test_2d_static<5, 5,      1, 0,     30, 30>();
    test_2d_static<5, 1,      0, 1,     30, 30>();
    test_2d_static<1, 5,      0, 1,     30, 30>();
    test_2d_static<5, 5,      0, 1,     30, 30>();
    test_2d_static<5, 1,      1, 0,     30, 30>();
    test_2d_static<1, 5,      1, 0,     30, 30>();
    test_2d_static<5, 5,      1, 0,     30, 30>();
    test_2d_static<5, 1,      1, 1,     30, 30>();
    test_2d_static<1, 5,      1, 1,     30, 30>();
    test_2d_static<5, 5,      1, 1,     30, 30>();

    ///////////////////////////////////////////////////////////////////////////
    // 2D Dynamic

    //              Striding   Padding   Dimensions
    test_2d_dynamic<5, 1,      1, 0,     30, 30>();
    test_2d_dynamic<1, 5,      1, 0,     30, 30>();
    test_2d_dynamic<5, 5,      1, 0,     30, 30>();
    test_2d_dynamic<5, 1,      0, 1,     30, 30>();
    test_2d_dynamic<1, 5,      0, 1,     30, 30>();
    test_2d_dynamic<5, 5,      0, 1,     30, 30>();
    test_2d_dynamic<5, 1,      1, 0,     30, 30>();
    test_2d_dynamic<1, 5,      1, 0,     30, 30>();
    test_2d_dynamic<5, 5,      1, 0,     30, 30>();
    test_2d_dynamic<5, 1,      1, 1,     30, 30>();
    test_2d_dynamic<1, 5,      1, 1,     30, 30>();

    test_2d_dynamic<5, 5,      1, 1,     30, 30>();
    test_2d_dynamic<5, 1,      1, 0,     30, 30>();
    test_2d_dynamic<1, 5,      1, 0,     30, 30>();
    test_2d_dynamic<5, 5,      1, 0,     30, 30>();
    test_2d_dynamic<5, 1,      0, 1,     30, 30>();
    test_2d_dynamic<1, 5,      0, 1,     30, 30>();
    test_2d_dynamic<5, 5,      0, 1,     30, 30>();
    test_2d_dynamic<5, 1,      1, 0,     30, 30>();
    test_2d_dynamic<1, 5,      1, 0,     30, 30>();
    test_2d_dynamic<5, 5,      1, 0,     30, 30>();
    test_2d_dynamic<5, 1,      1, 1,     30, 30>();
    test_2d_dynamic<1, 5,      1, 1,     30, 30>();
    test_2d_dynamic<5, 5,      1, 1,     30, 30>();

    return boost::report_errors();
}

