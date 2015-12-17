
#ifndef STD_EXPERIMENTAL_LAYOUTS
#define STD_EXPERIMENTAL_LAYOUTS

#include <ostream>

namespace std {
namespace experimental {

template< typename Dimension >
struct layout_offset_left
{
    using is_layout = std::true_type;
    using dimension = Dimension;
    dimension dim;

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

 layout_offset_left() = default ;
  layout_offset_left( const layout_offset_left & ) = default ;
  layout_offset_left( layout_offset_left && ) = default ;
  layout_offset_left & operator = ( const layout_offset_left & ) = default ;
  layout_offset_left & operator = ( layout_offset_left && ) = default ;

  constexpr layout_offset_left( size_t a0     , size_t a1 = 0 , size_t a2 = 0 , size_t a3 = 0 , size_t a4 = 0
                       , size_t a5 = 0 , size_t a6 = 0 , size_t a7 = 0 , size_t a8 = 0 , size_t a9 = 0 )
    : dim(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) {}
};

template< typename Dimension>
struct layout_offset_void
{
    using is_layout = std::true_type;
    using dimension = Dimension;
    dimension dim;
    using offset = layout_offset_void<Dimension>;

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

  layout_offset_void() = default ;
  layout_offset_void( const layout_offset_void & ) = default ;
  layout_offset_void( layout_offset_void && ) = default ;
  layout_offset_void & operator = ( const layout_offset_void & ) = default ;
  layout_offset_void & operator = ( layout_offset_void && ) = default ;

  constexpr layout_offset_void( size_t a0     , size_t a1 = 0 , size_t a2 = 0 , size_t a3 = 0 , size_t a4 = 0
                       , size_t a5 = 0 , size_t a6 = 0 , size_t a7 = 0 , size_t a8 = 0 , size_t a9 = 0 )
    : dim(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) {}
};

template< typename Dimension>
struct layout_offset_right
{
    using is_layout = std::true_type;
    using dimension = Dimension;
    dimension dim;
    using offset = layout_offset_right<Dimension>;

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

  layout_offset_right() = default ;
  layout_offset_right( const layout_offset_right & ) = default ;
  layout_offset_right( layout_offset_right && ) = default ;
  layout_offset_right & operator = ( const layout_offset_right & ) = default ;
  layout_offset_right & operator = ( layout_offset_right && ) = default ;

  constexpr layout_offset_right( size_t a0     , size_t a1 = 0 , size_t a2 = 0 , size_t a3 = 0 , size_t a4 = 0
                       , size_t a5 = 0 , size_t a6 = 0 , size_t a7 = 0 , size_t a8 = 0 , size_t a9 = 0 )
    : dim(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) {}
};

}} // namespace std::experimental

#endif

