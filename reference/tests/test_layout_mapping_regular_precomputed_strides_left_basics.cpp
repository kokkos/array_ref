////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include "detail/layout_mapping_regular_precomputed_strides.hpp"

using std::is_same;

using std::experimental::dyn;
using std::experimental::dimensions;
using std::experimental::detail::integer_sequence;
using std::experimental::detail::layout_mapping_regular_precomputed_strides;

int main()
{
    // Empty
    {
        layout_mapping_regular_precomputed_strides<
            dimensions<>                  // Dimensions
          , dimensions<>                  // Stepping
          , dimensions<>                  // Padding
          , integer_sequence<std::size_t> // Ordering
        > const lri;

        dimensions<> d;
        dimensions<> s;
        dimensions<> p;

        BOOST_TEST_EQ((d.rank()),         0);
        BOOST_TEST_EQ((d.rank_dynamic()), 0);


#if 0

        BOOST_TEST((is_same<
            decltype(lri)::otr_type, integer_sequence<std::size_t>
        >::value));
        BOOST_TEST((is_same<
            decltype(lri)::rto_type, integer_sequence<std::size_t>
        >::value));

        // NOTE: This instantiates the layout_mapping_regular_base specialization
        // for N == otr[0] - the SFINAE condition for  is_rank_unit_stride, which
        // is true for an empty integer_sequence. But, calling otr or rto is
        // always UB in this case as it violates their array bounds
        // preconditions, so we can't call is_unit_stride.

//        BOOST_TEST_EQ(true, (lri.is_unit_stride()));
//        BOOST_TEST_EQ(true, (decltype(lri)::is_unit_stride()));

        // is_dynamic_stride should avoid UB and return false. This implies
        // that dimensions::is_dynamic should return false when out of bounds.

        BOOST_TEST_EQ((d.is_dynamic(0)), false); // Out of bounds.
        BOOST_TEST_EQ((d.is_dynamic(3)), false); // Out of bounds.

        BOOST_TEST_EQ(false, (lri.is_dynamic_stride()));
        BOOST_TEST_EQ(false, (decltype(lri)::is_dynamic_stride()));

        // stride and index should avoid UB and return 0. This implies that
        // dimensions::operator[] return 0 and not dyn when out of bounds.

        BOOST_TEST_EQ((d[0]), 0); // Out of bounds.
        BOOST_TEST_EQ((d[3]), 0); // Out of bounds.

        int data = 42;
        int* dptr = &data;

        BOOST_TEST_EQ((lri.stride(d, s, p)), 0);

        BOOST_TEST_EQ((lri.index(d, s, p, {})), 0);

//        BOOST_TEST_EQ((dptr[lri.index(d, s, p, {})]), 42); 

//        BOOST_TEST_EQ(&(dptr[lri.index(d, s, p, {})]), dptr); 

//        dptr[lri.index(d, s, p, {})] = 17;

//        BOOST_TEST_EQ((dptr[lri.index(d, s, p, {})]), 17); 

#endif
    }

    return boost::report_errors();
}
