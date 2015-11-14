////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#include <boost/detail/lightweight_test.hpp>

#include <vector>

#include <multi_array_ref>

int main()
{
    using std::experimental::dims;
    using std::experimental::get_value;
    using std::experimental::column_major_padded_layout;
    constexpr auto dyn = std::experimental::dynamic_dim;

    {
        dims<> d;
        column_major_padded_layout<int, dims<> > l;

        int data = 42;
        int* dptr = &data;

        BOOST_TEST_EQ((l.index(d)), 0);

        BOOST_TEST_EQ((l.access(dptr, l.index(d))), 42); 
        BOOST_TEST_EQ(&(l.access(dptr, l.index(d))), dptr); 

        l.access(dptr, l.index(d)) = 17;

        BOOST_TEST_EQ((l.access(dptr, l.index(d))), 17); 
    }

    {
        dims<30> d;
        column_major_padded_layout<int, dims<0> > l;

        int dptr[30];

        for (auto i = 0; i < get_value<0>(d); ++i)
        {
            BOOST_TEST_EQ((l.index(d, i)), i);

            BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i))), &(dptr[i])); 

            l.access(dptr, l.index(d, i)) = 17;

            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 17); 
        }
    }

    {
        dims<dyn> d(30);
        column_major_padded_layout<int, dims<0> > l;

        std::vector<int> data(get_value<0>(d), 42);
        int* dptr = data.data();

        for (auto i = 0; i < get_value<0>(d); ++i)
        {
            BOOST_TEST_EQ((l.index(d, i)), i);

            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 42); 
            BOOST_TEST_EQ(&(l.access(dptr, l.index(d, i))), &(dptr[i])); 

            l.access(dptr, l.index(d, i)) = 17;

            BOOST_TEST_EQ((l.access(dptr, l.index(d, i))), 17); 
        }
    }

    return boost::report_errors();
}
