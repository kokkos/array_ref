////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/core/lightweight_test.hpp>

#include <mdspan>

using std::experimental::dyn;
using std::experimental::dimensions;

int main()
{
    {
        dimensions<3            > e0;
        dimensions<3,   5       > e1;
        dimensions<3,   5,   9  > e2;

        BOOST_TEST_EQ((e0[0]), 3);

        BOOST_TEST_EQ((e1[0]), 3);
        BOOST_TEST_EQ((e1[1]), 5);

        BOOST_TEST_EQ((e2[0]), 3);
        BOOST_TEST_EQ((e2[1]), 5);
        BOOST_TEST_EQ((e2[2]), 9);

        BOOST_TEST_EQ((e0.size()), 3    );
        BOOST_TEST_EQ((e1.size()), 3*5  );
        BOOST_TEST_EQ((e2.size()), 3*5*9);
    }

    {
        dimensions<dyn, 5,   9  > e0(3);
        dimensions<3,   dyn, 9  > e1(5);
        dimensions<3,   5,   dyn> e2(9);

        BOOST_TEST_EQ((e0[0]), 3);
        BOOST_TEST_EQ((e0[1]), 5);
        BOOST_TEST_EQ((e0[2]), 9);

        BOOST_TEST_EQ((e1[0]), 3);
        BOOST_TEST_EQ((e1[1]), 5);
        BOOST_TEST_EQ((e1[2]), 9);

        BOOST_TEST_EQ((e2[0]), 3);
        BOOST_TEST_EQ((e2[1]), 5);
        BOOST_TEST_EQ((e2[2]), 9);

        BOOST_TEST_EQ((e0.size()), 3*5*9);
        BOOST_TEST_EQ((e1.size()), 3*5*9);
        BOOST_TEST_EQ((e2.size()), 3*5*9);
    }


    {
        dimensions<3,   dyn, dyn> e0(5, 9);
        dimensions<dyn, 5,   dyn> e1(3, 9);
        dimensions<dyn, dyn, 9  > e2(3, 5);

        BOOST_TEST_EQ((e0[0]), 3);
        BOOST_TEST_EQ((e0[1]), 5);
        BOOST_TEST_EQ((e0[2]), 9);

        BOOST_TEST_EQ((e1[0]), 3);
        BOOST_TEST_EQ((e1[1]), 5);
        BOOST_TEST_EQ((e1[2]), 9);

        BOOST_TEST_EQ((e2[0]), 3);
        BOOST_TEST_EQ((e2[1]), 5);
        BOOST_TEST_EQ((e2[2]), 9);

        BOOST_TEST_EQ((e0.size()), 3*5*9);
        BOOST_TEST_EQ((e1.size()), 3*5*9);
        BOOST_TEST_EQ((e2.size()), 3*5*9);
    }

    {
        dimensions<dyn>           e0(3);
        dimensions<dyn, dyn>      e1(3, 5);
        dimensions<dyn, dyn, dyn> e2(3, 5, 9);

        BOOST_TEST_EQ((e0[0]), 3);

        BOOST_TEST_EQ((e1[0]), 3);
        BOOST_TEST_EQ((e1[1]), 5);

        BOOST_TEST_EQ((e2[0]), 3);
        BOOST_TEST_EQ((e2[1]), 5);
        BOOST_TEST_EQ((e2[2]), 9);

        BOOST_TEST_EQ((e0.size()), 3    );
        BOOST_TEST_EQ((e1.size()), 3*5  );
        BOOST_TEST_EQ((e2.size()), 3*5*9);
    }

    return boost::report_errors();
}

