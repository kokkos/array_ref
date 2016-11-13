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
using std::experimental::mdspan;

//#warning Better coverage for different types

//#warning Need coverage for const conversions/const pointer constructors

template <std::size_t X, typename T>
void test_empty_1d_static()
{ // {{{
    { // Rank 0 default constructed with no dimensions.
        mdspan<T> a;

        BOOST_TEST_EQ((a.rank()),         0);
        BOOST_TEST_EQ((a.rank_dynamic()), 0);
        BOOST_TEST_EQ((a.size()),         1);
        // Rank is 0; calling extent() with anything will be UB.
        BOOST_TEST_EQ((a.is_regular()),   true);
        // Rank is 0; calling stride() with anything will be UB.
    }

    { // Rank 0 default constructed with explicit dimensions<> template
      // parameter.
        mdspan<T, dimensions<> > a;

        BOOST_TEST_EQ((a.rank()),         0);
        BOOST_TEST_EQ((a.rank_dynamic()), 0);
        BOOST_TEST_EQ((a.size()),         1);
        // Rank is 0; calling extent() with anything will be UB.
        BOOST_TEST_EQ((a.is_regular()),   true);
        // Rank is 0; calling stride() with anything will be UB.
    }

    { // Rank 1 default constructed with C-array-style static dimension
      // declaration.
        mdspan<T[X]> a;

        BOOST_TEST_EQ((a.rank()),         1);
        BOOST_TEST_EQ((a.rank_dynamic()), 0);
        BOOST_TEST_EQ((a.size()),         X);
        BOOST_TEST_EQ((a.extent(0)),      X);
        BOOST_TEST_EQ((a.is_regular()),   true);
        BOOST_TEST_EQ((a.stride(0)),      1);
        BOOST_TEST_EQ((a.span()),         X);
    }

    { // Rank 1 default constructed with explicit dimensions<> parameter.
        mdspan<T, dimensions<X> > a;

        BOOST_TEST_EQ((a.rank()),         1);
        BOOST_TEST_EQ((a.rank_dynamic()), 0);
        BOOST_TEST_EQ((a.size()),         X);
        BOOST_TEST_EQ((a.extent(0)),      X);
        BOOST_TEST_EQ((a.is_regular()),   true);
        BOOST_TEST_EQ((a.stride(0)),      1);
        BOOST_TEST_EQ((a.span()),         X);
    }
} // }}}

template <std::size_t X>
void test_1d_static()
{ // {{{
    int dptr[X];

    mdspan<int,       dimensions<X> >  a(dptr);
    mdspan<int const, dimensions<X> > ca(dptr);

    BOOST_TEST_EQ((ca.rank()),         1);
    BOOST_TEST_EQ((ca.rank_dynamic()), 0);
    BOOST_TEST_EQ((ca.size()),         X);
    BOOST_TEST_EQ((ca.extent(0)),      X);
    BOOST_TEST_EQ((ca.is_regular()),   true);
    BOOST_TEST_EQ((ca.stride(0)),      1);
    BOOST_TEST_EQ((ca.span()),         X);

    // Set all elements to a unique value.
    for (auto i = 0; i < ca.extent(0); ++i)
    {
        BOOST_TEST_EQ(&(ca[i]), &(dptr[i])); 

        BOOST_TEST_EQ(&(ca(i)), &(dptr[i])); 

        a(i) = i;

        BOOST_TEST_EQ((ca[i]), i); 

        BOOST_TEST_EQ((ca(i)), i); 
    }
} // }}}

template <std::size_t X>
void test_1d_dynamic()
{ // {{{
    vector<int> data(X);
    int* dptr = data.data();

    mdspan<int,       dimensions<dyn> >  a(dptr, X);
    mdspan<int const, dimensions<dyn> > ca(dptr, X);

    BOOST_TEST_EQ((ca.rank()),         1);
    BOOST_TEST_EQ((ca.rank_dynamic()), 1);
    BOOST_TEST_EQ((ca.size()),         X);
    BOOST_TEST_EQ((ca.extent(0)),      X);
    BOOST_TEST_EQ((ca.is_regular()),   true);
    BOOST_TEST_EQ((ca.stride(0)),      1);
    BOOST_TEST_EQ((ca.span()),         X);

    // Set all elements to a unique value.
    for (auto i = 0; i < ca.extent(0); ++i)
    {
        BOOST_TEST_EQ(&(ca[i]), &(dptr[i])); 

        BOOST_TEST_EQ(&(ca(i)), &(dptr[i])); 

        a(i) = i;

        BOOST_TEST_EQ((ca[i]), i); 

        BOOST_TEST_EQ((ca(i)), i); 

        // Bounds-checking.
        BOOST_TEST_EQ((data.at(i)), i); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_static()
{ // {{{
    tuple<int, int> dptr[X * Y];

    mdspan<tuple<int, int>,       dimensions<X, Y> >  a(dptr);
    mdspan<tuple<int, int> const, dimensions<X, Y> > ca(dptr);

    BOOST_TEST_EQ((ca.rank()),         2);
    BOOST_TEST_EQ((ca.rank_dynamic()), 0);
    BOOST_TEST_EQ((ca.size()),         X * Y);
    BOOST_TEST_EQ((ca.extent(0)),      X);
    BOOST_TEST_EQ((ca.extent(1)),      Y);
    BOOST_TEST_EQ((ca.is_regular()),   true);
    BOOST_TEST_EQ((ca.stride(0)),      1);
    BOOST_TEST_EQ((ca.stride(1)),      1);
    BOOST_TEST_EQ((ca.span()),         X * Y);

    // Set all elements to a unique value.
    for (auto j = 0; j < ca.extent(1); ++j)
    for (auto i = 0; i < ca.extent(0); ++i)
    {
        auto const true_idx = (ca.extent(1)) * (i) + (j);

        BOOST_TEST_EQ(&(ca[true_idx]), &(dptr[true_idx])); 

        BOOST_TEST_EQ(&(ca(i, j)), &(dptr[true_idx])); 

        std::get<0>(a(i, j)) = i;
        std::get<1>(a(i, j)) = j;

        BOOST_TEST_EQ((std::get<0>(ca[true_idx])), i); 
        BOOST_TEST_EQ((std::get<1>(ca[true_idx])), j); 

        BOOST_TEST_EQ((std::get<0>(ca(i, j))), i); 
        BOOST_TEST_EQ((std::get<1>(ca(i, j))), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_dynamic()
{ // {{{
    vector<tuple<int, int> > data(X * Y);
    tuple<int, int>* dptr = data.data();

    mdspan<tuple<int, int>,       dimensions<dyn, dyn> >  a(dptr, X, Y);
    mdspan<tuple<int, int> const, dimensions<dyn, dyn> > ca(dptr, X, Y);

    BOOST_TEST_EQ((ca.rank()),         2);
    BOOST_TEST_EQ((ca.rank_dynamic()), 2);
    BOOST_TEST_EQ((ca.size()),         X * Y);
    BOOST_TEST_EQ((ca.extent(0)),      X);
    BOOST_TEST_EQ((ca.extent(1)),      Y);
    BOOST_TEST_EQ((ca.is_regular()),   true);
    BOOST_TEST_EQ((ca.stride(0)),      1);
    BOOST_TEST_EQ((ca.stride(1)),      1);
    BOOST_TEST_EQ((ca.span()),         X * Y);

    // Set all elements to a unique value.
    for (auto j = 0; j < ca.extent(1); ++j)
    for (auto i = 0; i < ca.extent(0); ++i)
    {
        auto const true_idx = (ca.extent(1)) * (i) + (j);

        BOOST_TEST_EQ(&(ca[true_idx]), &(dptr[true_idx])); 

        BOOST_TEST_EQ(&(ca(i, j)), &(dptr[true_idx])); 

        std::get<0>(a(i, j)) = i;
        std::get<1>(a(i, j)) = j;

        BOOST_TEST_EQ((std::get<0>(ca[true_idx])), i); 
        BOOST_TEST_EQ((std::get<1>(ca[true_idx])), j); 

        BOOST_TEST_EQ((std::get<0>(ca(i, j))), i); 
        BOOST_TEST_EQ((std::get<1>(ca(i, j))), j); 

        // Bounds-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(true_idx))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(true_idx))), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y>
void test_2d_mixed()
{ // {{{
    vector<tuple<int, int> > data(X * Y);
    tuple<int, int>* dptr = data.data();

    mdspan<tuple<int, int>,       dimensions<dyn, Y> >  a(dptr, X);
    mdspan<tuple<int, int> const, dimensions<dyn, Y> > ca(dptr, X);

    BOOST_TEST_EQ((ca.rank()),         2);
    BOOST_TEST_EQ((ca.rank_dynamic()), 1);
    BOOST_TEST_EQ((ca.size()),         X * Y);
    BOOST_TEST_EQ((ca.extent(0)),      X);
    BOOST_TEST_EQ((ca.extent(1)),      Y);
    BOOST_TEST_EQ((ca.is_regular()),   true);
    BOOST_TEST_EQ((ca.stride(0)),      1);
    BOOST_TEST_EQ((ca.stride(1)),      1);
    BOOST_TEST_EQ((ca.span()),         X * Y);

    // Set all elements to a unique value.
    for (auto j = 0; j < ca.extent(1); ++j)
    for (auto i = 0; i < ca.extent(0); ++i)
    {
        auto const true_idx = (ca.extent(1)) * (i) + (j);

        BOOST_TEST_EQ(&(ca[true_idx]), &(dptr[true_idx])); 

        BOOST_TEST_EQ(&(ca(i, j)), &(dptr[true_idx])); 

        std::get<0>(a(i, j)) = i;
        std::get<1>(a(i, j)) = j;

        BOOST_TEST_EQ((std::get<0>(ca[true_idx])), i); 
        BOOST_TEST_EQ((std::get<1>(ca[true_idx])), j); 

        BOOST_TEST_EQ((std::get<0>(ca(i, j))), i); 
        BOOST_TEST_EQ((std::get<1>(ca(i, j))), j); 

        // Bounds-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(true_idx))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(true_idx))), j); 
    }
} // }}}

template <std::size_t X, std::size_t Y, std::size_t Z>
void test_3d_static()
{ // {{{
    tuple<int, int, int> dptr[X * Y * Z];

    mdspan<tuple<int, int, int>,       dimensions<X, Y, Z> >  a(dptr);
    mdspan<tuple<int, int, int> const, dimensions<X, Y, Z> > ca(dptr);

    BOOST_TEST_EQ((ca.rank()),         3);
    BOOST_TEST_EQ((ca.rank_dynamic()), 0);
    BOOST_TEST_EQ((ca.size()),         X * Y * Z);
    BOOST_TEST_EQ((ca.extent(0)),      X);
    BOOST_TEST_EQ((ca.extent(1)),      Y);
    BOOST_TEST_EQ((ca.extent(2)),      Z);
    BOOST_TEST_EQ((ca.is_regular()),   true);
    BOOST_TEST_EQ((ca.stride(0)),      1);
    BOOST_TEST_EQ((ca.stride(1)),      1);
    BOOST_TEST_EQ((ca.stride(2)),      1);
    BOOST_TEST_EQ((ca.span()),         X * Y * Z);

    // Set all elements to a unique value.
    for (auto k = 0; k < ca.extent(2); ++k)
    for (auto j = 0; j < ca.extent(1); ++j)
    for (auto i = 0; i < ca.extent(0); ++i)
    {
        auto const m = ca.mapping();
        auto const true_idx = (m[1]) * (m[2]) * (i) + (m[2]) * (j) + (k);

        BOOST_TEST_EQ(&(ca[true_idx]), &(dptr[true_idx])); 

        BOOST_TEST_EQ(&(ca(i, j, k)), &(dptr[true_idx])); 

        std::get<0>(a(i, j, k)) = i;
        std::get<1>(a(i, j, k)) = j;
        std::get<2>(a(i, j, k)) = k;

        BOOST_TEST_EQ((std::get<0>(ca[true_idx])), i); 
        BOOST_TEST_EQ((std::get<1>(ca[true_idx])), j); 
        BOOST_TEST_EQ((std::get<2>(ca[true_idx])), k); 

        BOOST_TEST_EQ((std::get<0>(ca(i, j, k))), i); 
        BOOST_TEST_EQ((std::get<1>(ca(i, j, k))), j); 
        BOOST_TEST_EQ((std::get<2>(ca(i, j, k))), k); 
    }
} // }}}

template <std::size_t X, std::size_t Y, std::size_t Z>
void test_3d_dynamic()
{ // {{{
    vector<tuple<int, int, int> > data(X * Y * Z);
    tuple<int, int, int>* dptr = data.data();

    mdspan<tuple<int, int, int>,       dimensions<dyn, dyn, dyn> >
         a(dptr, X, Y, Z);
    mdspan<tuple<int, int, int> const, dimensions<dyn, dyn, dyn> >
        ca(dptr, X, Y, Z);

    BOOST_TEST_EQ((ca.rank()),         3);
    BOOST_TEST_EQ((ca.rank_dynamic()), 0);
    BOOST_TEST_EQ((ca.size()),         X * Y * Z);
    BOOST_TEST_EQ((ca.extent(0)),      X);
    BOOST_TEST_EQ((ca.extent(1)),      Y);
    BOOST_TEST_EQ((ca.extent(2)),      Z);
    BOOST_TEST_EQ((ca.is_regular()),   true);
    BOOST_TEST_EQ((ca.stride(0)),      1);
    BOOST_TEST_EQ((ca.stride(1)),      1);
    BOOST_TEST_EQ((ca.stride(2)),      1);
    BOOST_TEST_EQ((ca.span()),         X * Y * Z);

    // Set all elements to a unique value.
    for (auto k = 0; k < ca.extent(2); ++k)
    for (auto j = 0; j < ca.extent(1); ++j)
    for (auto i = 0; i < ca.extent(0); ++i)
    {
        auto const m = ca.mapping();
        auto const true_idx = (m[1]) * (m[2]) * (i) + (m[2]) * (j) + (k);

        BOOST_TEST_EQ(&(ca[true_idx]), &(dptr[true_idx])); 

        BOOST_TEST_EQ(&(ca(i, j, k)), &(dptr[true_idx])); 

        std::get<0>(a(i, j, k)) = i;
        std::get<1>(a(i, j, k)) = j;
        std::get<2>(a(i, j, k)) = k;

        BOOST_TEST_EQ((std::get<0>(ca[true_idx])), i); 
        BOOST_TEST_EQ((std::get<1>(ca[true_idx])), j); 
        BOOST_TEST_EQ((std::get<2>(ca[true_idx])), k); 

        BOOST_TEST_EQ((std::get<0>(ca(i, j, k))), i); 
        BOOST_TEST_EQ((std::get<1>(ca(i, j, k))), j); 
        BOOST_TEST_EQ((std::get<2>(ca(i, j, k))), k); 

        // Bounds-checking.
        BOOST_TEST_EQ((std::get<0>(data.at(true_idx))), i); 
        BOOST_TEST_EQ((std::get<1>(data.at(true_idx))), j); 
        BOOST_TEST_EQ((std::get<2>(data.at(true_idx))), k); 
    }
} // }}}

int main()
{
    // 1D Empty Static
    test_empty_1d_static<1,  int>();
    test_empty_1d_static<30, int>();

    // 1D Static
    test_1d_static<1 >();
    test_1d_static<30>();

    // 1D Dynamic
    test_1d_dynamic<1 >();
    test_1d_dynamic<30>();

    return boost::report_errors();
}
