////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <multi_array_ref>

int main()
{
    using std::rank;
    using std::extent;

    using std::experimental::dims;
    constexpr auto dyn = std::experimental::dynamic_dim;

    BOOST_TEST_EQ((rank<dims<       > >::value), 0);
    BOOST_TEST_EQ((rank<dims<3      > >::value), 1);
    BOOST_TEST_EQ((rank<dims<3, 5   > >::value), 2);
    BOOST_TEST_EQ((rank<dims<3, 5, 9> >::value), 3);
    
    BOOST_TEST_EQ((extent<dims<>   >::value), 0);
    BOOST_TEST_EQ((extent<dims<>, 0>::value), 0);
    BOOST_TEST_EQ((extent<dims<>, 1>::value), 0);
    BOOST_TEST_EQ((extent<dims<>, 2>::value), 0);
    
    BOOST_TEST_EQ((extent<dims<3>   >::value), 3);
    BOOST_TEST_EQ((extent<dims<3>, 0>::value), 3);
    BOOST_TEST_EQ((extent<dims<3>, 1>::value), 0);
    
    BOOST_TEST_EQ((extent<dims<3, 5>   >::value), 3);
    BOOST_TEST_EQ((extent<dims<3, 5>, 0>::value), 3);
    BOOST_TEST_EQ((extent<dims<3, 5>, 1>::value), 5);
    BOOST_TEST_EQ((extent<dims<3, 5>, 2>::value), 0);

    BOOST_TEST_EQ((extent<dims<dyn, 5>   >::value), dyn);
    BOOST_TEST_EQ((extent<dims<dyn, 5>, 0>::value), dyn);
    BOOST_TEST_EQ((extent<dims<dyn, 5>, 1>::value), 5);
    BOOST_TEST_EQ((extent<dims<dyn, 5>, 2>::value), 0);

    BOOST_TEST_EQ((sizeof(dims<       >)), 1);
    BOOST_TEST_EQ((sizeof(dims<3      >)), 1);
    BOOST_TEST_EQ((sizeof(dims<3, 5   >)), 1);
    BOOST_TEST_EQ((sizeof(dims<3, 5, 9>)), 1);

    BOOST_TEST_EQ((sizeof(dims<dyn          >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dims<dyn, dyn     >)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dims<dyn, dyn, dyn>)), 3*sizeof(std::size_t));

    BOOST_TEST_EQ((sizeof(dims<dyn, 5,   9  >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dims<3,   dyn, 9  >)), 1*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dims<3,   5,   dyn>)), 1*sizeof(std::size_t));

    BOOST_TEST_EQ((sizeof(dims<3,   dyn, dyn>)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dims<dyn, 5,   dyn>)), 2*sizeof(std::size_t));
    BOOST_TEST_EQ((sizeof(dims<dyn, dyn, 9  >)), 2*sizeof(std::size_t));

    return boost::report_errors();
}

