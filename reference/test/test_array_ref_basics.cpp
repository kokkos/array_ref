////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <array_ref>

using std::experimental::dyn;
using std::experimental::dimensions;
using std::experimental::array_ref;

template <std::size_t X, typename T>
void test_empty_1d_static()
{
    { // Rank 0 default constructed with no dimensions.
        array_ref<T> ar;

        BOOST_TEST_EQ((ar.rank()),                  0);
        BOOST_TEST_EQ((ar.rank_dynamic()),          0);
        BOOST_TEST_EQ((ar.size()),                  1);
        // Rank is 0; calling extent() with anything will be UB.
        BOOST_TEST_EQ((decltype(ar)::is_regular()), true);
        // Rank is 0; calling stride() with anything will be UB.
    }

    { // Rank 0 default constructed with explicit dimensions<> template
      // parameter.
        array_ref<T, dimensions<> > ar;

        BOOST_TEST_EQ((ar.rank()),                  0);
        BOOST_TEST_EQ((ar.rank_dynamic()),          0);
        BOOST_TEST_EQ((ar.size()),                  1);
        // Rank is 0; calling extent() with anything will be UB.
        BOOST_TEST_EQ((decltype(ar)::is_regular()), true);
        // Rank is 0; calling stride() with anything will be UB.
    }

    { // Rank 1 default constructed with C-array-style static dimension
      // declaration.
        array_ref<T[X]> ar;

        BOOST_TEST_EQ((ar.rank()),                  1);
        BOOST_TEST_EQ((ar.rank_dynamic()),          0);
        BOOST_TEST_EQ((ar.size()),                  X);
        BOOST_TEST_EQ((ar.extent(0)),               X);
        BOOST_TEST_EQ((decltype(ar)::is_regular()), true);
        BOOST_TEST_EQ((ar.stride(0)),               1);
        BOOST_TEST_EQ((ar.span()),                  X);
    }

    { // Rank 1 default constructed with explicit dimensions<> parameter.
        array_ref<T, dimensions<X> > ar;

        BOOST_TEST_EQ((ar.rank()),                  1);
        BOOST_TEST_EQ((ar.rank_dynamic()),          0);
        BOOST_TEST_EQ((ar.size()),                  X);
        BOOST_TEST_EQ((ar.extent(0)),               X);
        BOOST_TEST_EQ((decltype(ar)::is_regular()), true);
        BOOST_TEST_EQ((ar.stride(0)),               1);
        BOOST_TEST_EQ((ar.span()),                  X);
    }
}

int main()
{
    test_empty_1d_static<1,  int>();
    test_empty_1d_static<30, int>();

    return boost::report_errors();
}
