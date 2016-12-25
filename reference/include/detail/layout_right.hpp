///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015-2016 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#if !defined(STD_68717262_A4A1_41F8_94D6_FF64D30C3903)
#define STD_68717262_A4A1_41F8_94D6_FF64D30C3903

#include "detail/fwd.hpp"
#include "detail/layout_mapping_right.hpp"

namespace std { namespace experimental {

///////////////////////////////////////////////////////////////////////////////

struct layout_right
{
    template <std::size_t... Steps>
    struct stepping
    {
        template <std::size_t... Pads>
        struct padding
        {
            // Striding specified, padding specified.
            template <typename Dims>
            using mapping = layout_mapping_right<
                Dims
              , dimensions<Steps...>
              , dimensions<Pads...> 
            >;
        };

        // Striding specified, padding defaulted.
        template <typename Dims>
        using mapping = layout_mapping_right<
            Dims
          , dimensions<Steps...>
          , detail::make_filled_dims_t<Dims::rank(), 0>
        >;
    };

    template <std::size_t... Pads>
    struct padding
    {
        template <std::size_t... Steps>
        struct stepping
        {
            // Striding specified, padding specified.
            template <typename Dims>
            using mapping = layout_mapping_right<
                Dims
              , dimensions<Steps...>
              , dimensions<Pads...> 
            >;
        };

        // Striding defaulted, padding specified.
        template <typename Dims>
        using mapping = layout_mapping_right<
            Dims
          , detail::make_filled_dims_t<Dims::rank(), 1>
          , dimensions<Pads...>
        >;
    };

    // Striding defaulted, padding defaulted.
    template <typename Dims>
    using mapping = layout_mapping_right<
        Dims
      , detail::make_filled_dims_t<Dims::rank(), 1>
      , detail::make_filled_dims_t<Dims::rank(), 0>
    >;
};

}} // std::experimental

#endif // STD_68717262_A4A1_41F8_94D6_FF64D30C3903

