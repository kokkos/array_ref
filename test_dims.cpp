////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include "view.hpp"

int main()
{
    using std::rank;
    using std::extent;

    using boost::extents;
    constexpr auto ext = boost::dynamic_extent;

    BOOST_TEST_EQ((rank<extents<       > >::value), 0);
    BOOST_TEST_EQ((rank<extents<3      > >::value), 1);
    BOOST_TEST_EQ((rank<extents<3, 5   > >::value), 2);
    BOOST_TEST_EQ((rank<extents<3, 5, 9> >::value), 3);
    
    BOOST_TEST_EQ((extent<extents<>   >::value), 0);
    BOOST_TEST_EQ((extent<extents<>, 0>::value), 0);
    BOOST_TEST_EQ((extent<extents<>, 1>::value), 0);
    BOOST_TEST_EQ((extent<extents<>, 2>::value), 0);
    
    BOOST_TEST_EQ((extent<extents<3>   >::value), 3);
    BOOST_TEST_EQ((extent<extents<3>, 0>::value), 3);
    BOOST_TEST_EQ((extent<extents<3>, 1>::value), 0);
    
    BOOST_TEST_EQ((extent<extents<3, 5>   >::value), 3);
    BOOST_TEST_EQ((extent<extents<3, 5>, 0>::value), 3);
    BOOST_TEST_EQ((extent<extents<3, 5>, 1>::value), 5);
    BOOST_TEST_EQ((extent<extents<3, 5>, 2>::value), 0);

    BOOST_TEST_EQ((extent<extents<ext, 5>   >::value), ext);
    BOOST_TEST_EQ((extent<extents<ext, 5>, 0>::value), ext);
    BOOST_TEST_EQ((extent<extents<ext, 5>, 1>::value), 5);
    BOOST_TEST_EQ((extent<extents<ext, 5>, 2>::value), 0);

    BOOST_TEST_EQ((sizeof(extents<       >)), 1);
    BOOST_TEST_EQ((sizeof(extents<3      >)), 1);
    BOOST_TEST_EQ((sizeof(extents<3, 5   >)), 1);
    BOOST_TEST_EQ((sizeof(extents<3, 5, 9>)), 1);

    BOOST_TEST_EQ((sizeof(extents<ext          >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(extents<ext, ext     >)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(extents<ext, ext, ext>)), 3*sizeof(std::size_t));

    BOOST_TEST_EQ((sizeof(extents<ext, 5,   9  >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(extents<3,   ext, 9  >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(extents<3,   5,   ext>)), 1*sizeof(std::size_t));

    BOOST_TEST_EQ((sizeof(extents<3,   ext, ext>)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(extents<ext, 5,   ext>)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(extents<ext, ext, 9  >)), 2*sizeof(std::size_t));

    return boost::report_errors();
}

