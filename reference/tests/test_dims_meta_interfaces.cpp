////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::rank;
using std::extent;

using std::experimental::dyn;
using std::experimental::dimensions;

int main()
{
    BOOST_TEST_EQ((rank<dimensions<       > >::value), 0);
    BOOST_TEST_EQ((rank<dimensions<3      > >::value), 1);
    BOOST_TEST_EQ((rank<dimensions<3, 5   > >::value), 2);
    BOOST_TEST_EQ((rank<dimensions<3, 5, 9> >::value), 3);
    
    BOOST_TEST_EQ((dimensions<       >::rank()), 0);
    BOOST_TEST_EQ((dimensions<3      >::rank()), 1);
    BOOST_TEST_EQ((dimensions<3, 5   >::rank()), 2);
    BOOST_TEST_EQ((dimensions<3, 5, 9>::rank()), 3);
    
    BOOST_TEST_EQ((extent<dimensions<>   >::value), 0);
    BOOST_TEST_EQ((extent<dimensions<>, 0>::value), 0);
    BOOST_TEST_EQ((extent<dimensions<>, 1>::value), 0);
    BOOST_TEST_EQ((extent<dimensions<>, 2>::value), 0);
    
    BOOST_TEST_EQ((extent<dimensions<3>   >::value), 3);
    BOOST_TEST_EQ((extent<dimensions<3>, 0>::value), 3);
    BOOST_TEST_EQ((extent<dimensions<3>, 1>::value), 0);

    BOOST_TEST_EQ((extent<dimensions<0>   >::value), 0);
    BOOST_TEST_NE((extent<dimensions<0>   >::value), dyn);
    BOOST_TEST_EQ((extent<dimensions<0>, 0>::value), 0);
    BOOST_TEST_NE((extent<dimensions<0>, 0>::value), dyn);
    BOOST_TEST_EQ((extent<dimensions<0>, 1>::value), 0);
    BOOST_TEST_NE((extent<dimensions<0>, 1>::value), dyn);
    
    BOOST_TEST_EQ((extent<dimensions<3, 5>   >::value), 3);
    BOOST_TEST_EQ((extent<dimensions<3, 5>, 0>::value), 3);
    BOOST_TEST_EQ((extent<dimensions<3, 5>, 1>::value), 5);
    BOOST_TEST_EQ((extent<dimensions<3, 5>, 2>::value), 0);

    BOOST_TEST_EQ((extent<dimensions<dyn, 5>   >::value), dyn);
    BOOST_TEST_EQ((extent<dimensions<dyn, 5>, 0>::value), dyn);
    BOOST_TEST_NE((extent<dimensions<dyn, 5>   >::value), 0);
    BOOST_TEST_NE((extent<dimensions<dyn, 5>, 0>::value), 0);
    BOOST_TEST_EQ((extent<dimensions<dyn, 5>, 1>::value), 5);
    BOOST_TEST_EQ((extent<dimensions<dyn, 5>, 2>::value), 0);

    BOOST_TEST_EQ((sizeof(dimensions<       >)), 1);
    BOOST_TEST_EQ((sizeof(dimensions<3      >)), 1);
    BOOST_TEST_EQ((sizeof(dimensions<3, 5   >)), 1);
    BOOST_TEST_EQ((sizeof(dimensions<3, 5, 9>)), 1);

    BOOST_TEST_EQ((sizeof(dimensions<dyn          >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dimensions<dyn, dyn     >)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dimensions<dyn, dyn, dyn>)), 3*sizeof(std::size_t));

    BOOST_TEST_EQ((sizeof(dimensions<dyn, 5,   9  >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dimensions<3,   dyn, 9  >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dimensions<3,   5,   dyn>)), 1*sizeof(std::size_t));

    BOOST_TEST_EQ((sizeof(dimensions<3,   dyn, dyn>)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dimensions<dyn, 5,   dyn>)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dimensions<dyn, dyn, 9  >)), 2*sizeof(std::size_t));

    return boost::report_errors();
}

