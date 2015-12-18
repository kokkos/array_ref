
#ifndef STD_EXPERIMENTAL_LAYOUTS
#define STD_EXPERIMENTAL_LAYOUTS

#include <ostream>

namespace std {
namespace experimental {

template< typename Dimension >
struct offset_left
{
  using dim_type = Dimension;
  dim_type dim;

  using is_regular = std::true_type ;

  template< typename t0 >
  constexpr size_t operator()( const t0 & i0 ) const
    { return i0 ; }

  template< typename t0 , typename t1 >
  constexpr size_t operator()( const t0 & i0 , const t1 & i1 ) const
    { return i0 + dim.n0 * i1 ; }

  template< typename t0 , typename t1 , typename t2 >
  constexpr size_t operator()( const t0 & i0 , const t1 & i1 , const t2 & i2 ) const
    { return i0 + dim.n0 * ( i1 + dim.n1 * i2 ); }

  constexpr size_t stride_0() const { return 1 ; }
  constexpr size_t stride_1() const { return dim.n0 ; }
  constexpr size_t stride_2() const { return dim.n0 * dim.n1 ; }

  constexpr size_t span() const
    { return dim.n0 * dim.n1 * dim.n2 * dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }

  offset_left() = default ;
  offset_left( const offset_left & ) = default ;
  offset_left( offset_left && ) = default ;
  offset_left & operator = ( const offset_left & ) = default ;
  offset_left & operator = ( offset_left && ) = default ;

  constexpr offset_left( size_t a0     , size_t a1 = 0 , size_t a2 = 0 , size_t a3 = 0 , size_t a4 = 0
                       , size_t a5 = 0 , size_t a6 = 0 , size_t a7 = 0 , size_t a8 = 0 , size_t a9 = 0 )
    : dim(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) {}
};

template< typename Dimension >
struct offset_void
{
  using dim_type = Dimension;
  dim_type dim;

  using is_regular = std::true_type ;

  template< typename t0 >
  constexpr size_t operator()( const t0 & i0 ) const
    { return i0 ; }

  template< typename t0 , typename t1 >
  constexpr size_t operator()( const t0 & i0 , const t1 & i1 ) const
    { return i0 * dim.n1 + i1 ; }

  template< typename t0 , typename t1 , typename t2 >
  constexpr size_t operator()( const t0 & i0 , const t1 & i1 , const t2 & i2 ) const
    { return ( i0 * dim.n1 + i1 ) * dim.n2 + i2 ; }

  constexpr size_t stride_0() const { return dim.n1 * dim.n2 * dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }
  constexpr size_t stride_1() const { return dim.n2 * dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }
  constexpr size_t stride_2() const { return dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }
  constexpr size_t stride_3() const { return dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }

  constexpr size_t span() const
    { return dim.n0 * dim.n1 * dim.n2 * dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }

  offset_void() = default ;
  offset_void( const offset_void & ) = default ;
  offset_void( offset_void && ) = default ;
  offset_void & operator = ( const offset_void & ) = default ;
  offset_void & operator = ( offset_void && ) = default ;

  constexpr offset_void( size_t a0     , size_t a1 = 0 , size_t a2 = 0 , size_t a3 = 0 , size_t a4 = 0
                       , size_t a5 = 0 , size_t a6 = 0 , size_t a7 = 0 , size_t a8 = 0 , size_t a9 = 0 )
    : dim(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) {}
};

template< typename Dimension >
struct offset_right
{
  using dim_type = Dimension;
  dim_type dim;

  using is_regular = std::true_type ;

  template< typename t0 >
  constexpr size_t operator()( const t0 & i0 ) const
    { return i0 ; }

  template< typename t0 , typename t1 >
  constexpr size_t operator()( const t0 & i0 , const t1 & i1 ) const
    { return i0 * dim.n1 + i1 ; }

  template< typename t0 , typename t1 , typename t2 >
  constexpr size_t operator()( const t0 & i0 , const t1 & i1 , const t2 & i2 ) const
    { return ( i0 * dim.n1 + i1 ) * dim.n2 + i2 ; }

  constexpr size_t stride_0() const { return dim.n1 * dim.n2 * dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }
  constexpr size_t stride_1() const { return dim.n2 * dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }
  constexpr size_t stride_2() const { return dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }
  constexpr size_t stride_3() const { return dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }

  constexpr size_t span() const
    { return dim.n0 * dim.n1 * dim.n2 * dim.n3 * dim.n4 * dim.n5 * dim.n6 * dim.n7 * dim.n8 * dim.n9 ; }

  offset_right() = default ;
  offset_right( const offset_right & ) = default ;
  offset_right( offset_right && ) = default ;
  offset_right & operator = ( const offset_right & ) = default ;
  offset_right & operator = ( offset_right && ) = default ;

  constexpr offset_right( size_t a0     , size_t a1 = 0 , size_t a2 = 0 , size_t a3 = 0 , size_t a4 = 0
                       , size_t a5 = 0 , size_t a6 = 0 , size_t a7 = 0 , size_t a8 = 0 , size_t a9 = 0 )
    : dim(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) {}
};

struct layout_void {
    template< typename Dimension >
    using offset = offset_void<Dimension>;
};

struct layout_left {
    template< typename Dimension >
    using offset = offset_left<Dimension>;
};

struct layout_right{
    template< typename Dimension >
    using offset = offset_right<Dimension>;
};

struct layout_stride {
//     template< typename Dimension >
//     using offset = offset_stride<Dimension>;
};

}} // namespace std::experimental

#endif

