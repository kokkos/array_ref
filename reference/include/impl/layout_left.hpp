///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E)
#define STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E

#include "impl/fwd.hpp"
#include "impl/basic_layout_left.hpp"

namespace std { namespace experimental
{

struct layout_left
{
    template <std::size_t... Strides>
    struct striding
    {
        template <std::size_t... Pads>
        struct padding
        {
            // Striding specified, padding specified.
            template <typename Dims>
            using mapping = basic_layout_left<
                Dims
              , dimensions<Strides...>
              , dimensions<Pads...> 
            >;
        };

        // Striding specified, padding defaulted.
        template <typename Dims>
        using mapping = basic_layout_left<
            Dims
          , dimensions<Strides...>
          , detail::make_filled_dims_t<Dims::rank(), 0>
        >;
    };

    template <std::size_t... Pads>
    struct padding
    {
        template <std::size_t... Strides>
        struct striding
        {
            // Striding specified, padding specified.
            template <typename Dims>
            using mapping = basic_layout_left<
                Dims
              , dimensions<Strides...>
              , dimensions<Pads...> 
            >;
        };

        // Striding defaulted, padding specified.
        template <typename Dims>
        using mapping = basic_layout_left<
            Dims
          , detail::make_filled_dims_t<Dims::rank(), 1>
          , dimensions<Pads...>
        >;
    };

    // Striding defaulted, padding defaulted.
    template <typename Dims>
    using mapping = basic_layout_left<
        Dims
      , detail::make_filled_dims_t<Dims::rank(), 1>
      , detail::make_filled_dims_t<Dims::rank(), 0>
    >;
};

}} // std::experimental

#endif // STD_1147DB06_B234_4218_9C7F_0E1EAEF9B78E

