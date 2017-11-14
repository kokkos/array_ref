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

template <
    std::size_t StepX, std::size_t StepY 
  , std::size_t PadX, std::size_t PadY 
  , std::size_t X, std::size_t Y
    >
void test_2d_static()
{ // {{{
    static_assert(0 == (X % StepX), "X must be divisable by StepX");
    static_assert(0 == (Y % StepY), "Y must be divisable by StepY");

    layout_mapping_right<
        dimensions<X, Y>
      , dimensions<1, 1>
      , dimensions<PadX, PadY>
    > const l{};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), (X + PadX) * (Y + PadY));

    layout_mapping_right<
        dimensions<X / StepX, Y / StepY>
      , dimensions<StepX, StepY>
      , dimensions<PadX, PadY>
    > const sub_l{};

    BOOST_TEST_EQ((sub_l.is_regular()), true);

    BOOST_TEST_EQ((sub_l.stride(0)), StepX);
    BOOST_TEST_EQ((sub_l.stride(1)), StepY);

    BOOST_TEST_EQ((sub_l.size()), (X / StepX) * (Y / StepY));
    BOOST_TEST_EQ((sub_l.span()), (X + PadX) * (Y + PadY));

    int dptr[(X + PadX) * (Y + PadY)];

    // Set all real elements to 42.
    for (auto j = 0; j < l[1]; ++j)
    for (auto i = 0; i < l[0]; ++i)
    {
        auto const true_idx = (l[1] + l.padding()[1]) * (i) + (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx]));

        dptr[l.index(i, j)] = 42;

        BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);
    }

    // Set X pad elements to 17 and Y pad elements to 24. 
    for (auto j = 0; j < l[1] + l.padding()[1]; ++j)
    for (auto i = 0; i < l[0] + l.padding()[0]; ++i)
    {
        auto const true_idx = (l[1] + l.padding()[1]) * (i) + (j);

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

    // Set every (StepXth, StepYth) element to 71.
    for (auto j = 0; j < sub_l[1]; ++j)
    for (auto i = 0; i < sub_l[0]; ++i)
    {
        auto const p = l.padding();
        auto const s = sub_l.stepping();
        auto const true_idx = (sub_l[1] * s[1] + p[1]) * (s[0] * i) + (s[1] * j);

        BOOST_TEST_EQ((sub_l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[sub_l.index(i, j)]), &(dptr[true_idx])); 

        dptr[sub_l.index(i, j)] = 71;

        BOOST_TEST_EQ((dptr[sub_l.index(i, j)]), 71);
    }

    // Check final structure. 
    for (auto j = 0; j < l[1] + l.padding()[1]; ++j)
    for (auto i = 0; i < l[0] + l.padding()[0]; ++i)
    {
        auto const true_idx = (l[1] + l.padding()[1]) * (i) + (j);

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
            // Real element in the strided sub-box.
            if (  (0 == (i % sub_l.stepping()[0]))
               && (0 == (j % sub_l.stepping()[1]))
               )
            {
                BOOST_TEST_EQ((dptr[l.index(i, j)]), 71);
            }
            // Real element not in the strided sub-box.
            else
            {
                BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);
            }
        }
    }
} // }}}

template <
    std::size_t StepX, std::size_t StepY 
  , std::size_t PadX, std::size_t PadY 
  , std::size_t X, std::size_t Y
    >
void test_2d_dynamic()
{ // {{{
    layout_mapping_right<
        dimensions<dyn, dyn>
      , dimensions<dyn, dyn>
      , dimensions<dyn, dyn>
    > const l{{X, Y}, {1, 1}, {PadX, PadY}};

    BOOST_TEST_EQ((l.is_regular()), true);

    BOOST_TEST_EQ((l.stride(0)), 1);
    BOOST_TEST_EQ((l.stride(1)), 1);

    BOOST_TEST_EQ((l.size()), X * Y);
    BOOST_TEST_EQ((l.span()), (X + PadX) * (Y + PadY));

    layout_mapping_right<
        dimensions<dyn, dyn>
      , dimensions<dyn, dyn>
      , dimensions<dyn, dyn>
    > const sub_l{{X / StepX, Y / StepY}, {StepX, StepY}, {PadX, PadY}};

    BOOST_TEST_EQ((sub_l.is_regular()), true);

    BOOST_TEST_EQ((sub_l.stride(0)), StepX);
    BOOST_TEST_EQ((sub_l.stride(1)), StepY);

    BOOST_TEST_EQ((sub_l.size()), (X / StepX) * (Y / StepY));
    BOOST_TEST_EQ((sub_l.span()), (X + PadX) * (Y + PadY));

    // Initialize all elements to 42.
    std::vector<int> data(
        (l[0] + l.padding()[0]) * (l[1] + l.padding()[1]), 42
    );
    int* dptr = data.data();

    // Set X pad elements to 17 and Y pad elements to 24. 
    for (auto j = 0; j < l[1] + l.padding()[1]; ++j)
    for (auto i = 0; i < l[0] + l.padding()[0]; ++i)
    {
        auto const true_idx = (l[1] + l.padding()[1]) * (i) + (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx])); 

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

    // Set every (StepXth, StepYth) element to 71.
    for (auto j = 0; j < sub_l[1]; ++j)
    for (auto i = 0; i < sub_l[0]; ++i)
    {
        auto const p = l.padding();
        auto const s = sub_l.stepping();
        auto const true_idx = (sub_l[1] * s[1] + p[1]) * (s[0] * i) + (s[1] * j);

        BOOST_TEST_EQ((sub_l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[sub_l.index(i, j)]), &(dptr[true_idx])); 

        dptr[sub_l.index(i, j)] = 71;

        BOOST_TEST_EQ((dptr[sub_l.index(i, j)]), 71);

        // Bounds-checking.
        BOOST_TEST_EQ((data.at(sub_l.index(i, j))), 71);
    }

    // Check final structure. 
    for (auto j = 0; j < l[1] + l.padding()[1]; ++j)
    for (auto i = 0; i < l[0] + l.padding()[0]; ++i)
    {
        auto const true_idx = (l[1] + l.padding()[1]) * (i) + (j);

        BOOST_TEST_EQ((l.index(i, j)), true_idx);

        BOOST_TEST_EQ(&(dptr[l.index(i, j)]), &(dptr[true_idx])); 

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
            // Real element in the strided sub-box.
            if (  (0 == (i % sub_l.stepping()[0]))
               && (0 == (j % sub_l.stepping()[1]))
               )
            {
                BOOST_TEST_EQ((dptr[l.index(i, j)]), 71);

                // Bounds-checking.
                BOOST_TEST_EQ((data.at(l.index(i, j))), 71);
            }
            // Real element not in the strided sub-box.
            else
            {
                BOOST_TEST_EQ((dptr[l.index(i, j)]), 42);

                // Bounds-checking.
                BOOST_TEST_EQ((data.at(l.index(i, j))), 42);
            }
        }
    }
} // }}}

int main()
{
    ///////////////////////////////////////////////////////////////////////////
    // 2D Static

    //             Stepping   Padding   Dimensions
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

    //              Stepping   Padding   Dimensions
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

