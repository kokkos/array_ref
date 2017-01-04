////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include "detail/layout_regular_indexer.hpp"

using std::is_same;

using std::experimental::dyn;
using std::experimental::dimensions;
using std::experimental::detail::integer_sequence;
using std::experimental::detail::layout_regular_indexer;

int main()
{
    // Empty
    {
        layout_regular_indexer<
            0
          , dimensions<>                  // Dimensions
          , dimensions<>                  // Stepping
          , dimensions<>                  // Padding
          , integer_sequence<std::size_t> // Ordering
        > const lri;

        dimensions<> d;
        dimensions<> s;
        dimensions<> p;

        // Not sure wtf this does but :)

        // NOTE: This instantiates the layout_regular_indexer specialization
        // for N == otr[0] - the SFINAE condition is is_rank_unit_stride, which
        // is true for an empty integer_sequence.

        BOOST_TEST((is_same<
            decltype(lri)::otr_type, integer_sequence<std::size_t>
        >::value));
        BOOST_TEST((is_same<
            decltype(lri)::rto_type, integer_sequence<std::size_t>
        >::value));

        // Calling otr or rto is always UB in this case as it violates their
        // array bounds preconditions.

        BOOST_TEST_EQ(true, (lri.is_unit_stride()));
        BOOST_TEST_EQ(true, (decltype(lri)::is_unit_stride()));

        // is_dynamic_stride should avoid UB and return false.  stride and
        // index should avoid UB and return 0. This implies that
        // dimensions::is_dynamic and dimensions::operator[] return 0.

        BOOST_TEST_EQ(false, (lri.is_dynamic_stride()));
        BOOST_TEST_EQ(false, (decltype(lri)::is_dynamic_stride()));

/*
        BOOST_TEST_EQ((lri.is_regular()), true);

        BOOST_TEST_EQ((l.size()), 1);
        BOOST_TEST_EQ((l.span()), 1);

        int data = 42;
        int* dptr = &data;

        BOOST_TEST_EQ((l.index()), 0);

        BOOST_TEST_EQ((dptr[l.index()]), 42); 

        BOOST_TEST_EQ(&(dptr[l.index()]), dptr); 

        dptr[l.index()] = 17;

        BOOST_TEST_EQ((dptr[l.index()]), 17); 
*/
    }

    return boost::report_errors();
}
