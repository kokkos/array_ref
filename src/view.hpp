
#ifndef STD_EXPERIMENTAL_VIEW
#define STD_EXPERIMENTAL_VIEW

#include <limits>
#include <ostream>
#include <layouts.hpp>

namespace std {
namespace experimental {
namespace view_property {

// For explicit dimensions either
// static constexpr size_t
// enum : size_t 

#define VIEW_DIMENSION_DECLARATION_MACRO( R ) \
template< size_t ExN > struct dimension_ ## R \
{ \
  static constexpr size_t extent_ ## R = ( ExN < std::numeric_limits<size_t>::max() ? ExN : 1 ); \
  static constexpr size_t n ## R = ( ExN < std::numeric_limits<size_t>::max() ? ExN : 1 ); \
  dimension_ ## R () = default ; \
  dimension_ ## R ( const dimension_ ## R & ) = default ; \
  dimension_ ## R ( dimension_ ## R && ) = default ; \
  dimension_ ## R & operator = ( const dimension_ ## R & ) = default ; \
  dimension_ ## R & operator = ( dimension_ ## R && ) = default ; \
  constexpr dimension_ ## R ( size_t ) {} \
}; \
\
template<> struct dimension_ ## R <0> \
{ \
  static constexpr size_t extent_ ## R = 0 ; \
  size_t n ## R ; \
  dimension_ ## R () = default ; \
  dimension_ ## R ( const dimension_ ## R & ) = default ; \
  dimension_ ## R ( dimension_ ## R && ) = default ; \
  dimension_ ## R & operator = ( const dimension_ ## R & ) = default ; \
  dimension_ ## R & operator = ( dimension_ ## R && ) = default ; \
  constexpr dimension_ ## R ( size_t a ) : n ## R ( a ) {} \
};


VIEW_DIMENSION_DECLARATION_MACRO(0)
VIEW_DIMENSION_DECLARATION_MACRO(1)
VIEW_DIMENSION_DECLARATION_MACRO(2)
VIEW_DIMENSION_DECLARATION_MACRO(3)
VIEW_DIMENSION_DECLARATION_MACRO(4)
VIEW_DIMENSION_DECLARATION_MACRO(5)
VIEW_DIMENSION_DECLARATION_MACRO(6)
VIEW_DIMENSION_DECLARATION_MACRO(7)
VIEW_DIMENSION_DECLARATION_MACRO(8)
VIEW_DIMENSION_DECLARATION_MACRO(9)

#undef VIEW_DIMENSION_DECLARATION_MACRO

template< size_t ExN0 = std::numeric_limits<size_t>::max()
        , size_t ExN1 = std::numeric_limits<size_t>::max()
        , size_t ExN2 = std::numeric_limits<size_t>::max()
        , size_t ExN3 = std::numeric_limits<size_t>::max()
        , size_t ExN4 = std::numeric_limits<size_t>::max()
        , size_t ExN5 = std::numeric_limits<size_t>::max()
        , size_t ExN6 = std::numeric_limits<size_t>::max()
        , size_t ExN7 = std::numeric_limits<size_t>::max()
        , size_t ExN8 = std::numeric_limits<size_t>::max()
        , size_t ExN9 = std::numeric_limits<size_t>::max()
        >
struct dimension
  : dimension_0<ExN0>
  , dimension_1<ExN1>
  , dimension_2<ExN2>
  , dimension_3<ExN3>
  , dimension_4<ExN4>
  , dimension_5<ExN5>
  , dimension_6<ExN6>
  , dimension_7<ExN7>
  , dimension_8<ExN8>
  , dimension_9<ExN9>
{
  static constexpr unsigned rank =
    ( ExN0 == std::numeric_limits<size_t>::max() ? 0 :
    ( ExN1 == std::numeric_limits<size_t>::max() ? 1 :
    ( ExN2 == std::numeric_limits<size_t>::max() ? 2 :
    ( ExN3 == std::numeric_limits<size_t>::max() ? 3 :
    ( ExN4 == std::numeric_limits<size_t>::max() ? 4 :
    ( ExN5 == std::numeric_limits<size_t>::max() ? 5 :
    ( ExN6 == std::numeric_limits<size_t>::max() ? 6 :
    ( ExN7 == std::numeric_limits<size_t>::max() ? 7 :
    ( ExN8 == std::numeric_limits<size_t>::max() ? 8 :
    ( ExN9 == std::numeric_limits<size_t>::max() ? 9 : 10 ))))))))));

  using dimension_0<ExN0>::extent_0 ;
  using dimension_1<ExN1>::extent_1 ;
  using dimension_2<ExN2>::extent_2 ;
  using dimension_3<ExN3>::extent_3 ;
  using dimension_4<ExN4>::extent_4 ;
  using dimension_5<ExN5>::extent_5 ;
  using dimension_6<ExN6>::extent_6 ;
  using dimension_7<ExN7>::extent_7 ;
  using dimension_8<ExN8>::extent_8 ;
  using dimension_9<ExN9>::extent_9 ;

  using dimension_0<ExN0>::n0 ;
  using dimension_1<ExN1>::n1 ;
  using dimension_2<ExN2>::n2 ;
  using dimension_3<ExN3>::n3 ;
  using dimension_4<ExN4>::n4 ;
  using dimension_5<ExN5>::n5 ;
  using dimension_6<ExN6>::n6 ;
  using dimension_7<ExN7>::n7 ;
  using dimension_8<ExN8>::n8 ;
  using dimension_9<ExN9>::n9 ;

private:

  enum : unsigned
    { D0 = 0
    , D1 = D0 + ( ExN0 != 0 ? 0 : 1 )
    , D2 = D1 + ( ExN1 != 0 ? 0 : 1 )
    , D3 = D2 + ( ExN2 != 0 ? 0 : 1 )
    , D4 = D3 + ( ExN3 != 0 ? 0 : 1 )
    , D5 = D4 + ( ExN4 != 0 ? 0 : 1 )
    , D6 = D5 + ( ExN5 != 0 ? 0 : 1 )
    , D7 = D6 + ( ExN6 != 0 ? 0 : 1 )
    , D8 = D7 + ( ExN7 != 0 ? 0 : 1 )
    , D9 = D8 + ( ExN8 != 0 ? 0 : 1 )
    };

public:

  dimension() = default ;
  dimension( const dimension & ) = default ;
  dimension( dimension && ) = default ;
  dimension & operator = ( const dimension & ) = default ;
  dimension & operator = ( dimension && ) = default ;

  constexpr dimension( size_t a0     , size_t a1 = 0 , size_t a2 = 0 , size_t a3 = 0 , size_t a4 = 0
                     , size_t a5 = 0 , size_t a6 = 0 , size_t a7 = 0 , size_t a8 = 0 , size_t a9 = 0 )
    : dimension_0<ExN0>( a0 )
    , dimension_1<ExN1>( D1 == 0 ? a0 : a1 )
    , dimension_2<ExN2>( D2 == 0 ? a0 : ( D2 == 1 ? a1 : a2 ) )
    , dimension_3<ExN3>( D3 == 0 ? a0 : ( D3 == 1 ? a1 : ( D3 == 2 ? a2 : a3 )))
    , dimension_4<ExN4>( D4 == 0 ? a0 : ( D4 == 1 ? a1 : ( D4 == 2 ? a2 : ( D4 == 3 ? a3 : a4 ))))
    , dimension_5<ExN5>( D5 == 0 ? a0 : ( D5 == 1 ? a1 : ( D5 == 2 ? a2 : ( D5 == 3 ? a3 : ( D5 == 4 ? a4 : a5 )))))
    , dimension_6<ExN6>( D6 == 0 ? a0 : ( D6 == 1 ? a1 : ( D6 == 2 ? a2 : ( D6 == 3 ? a3 :
                       ( D6 == 4 ? a4 : ( D6 == 5 ? a5 : a6 ))))))
    , dimension_7<ExN7>( D7 == 0 ? a0 : ( D7 == 1 ? a1 : ( D7 == 2 ? a2 : ( D7 == 3 ? a3 :
                       ( D7 == 4 ? a4 : ( D7 == 5 ? a5 : ( D7 == 6 ? a6 : a7 )))))))
    , dimension_8<ExN8>( D8 == 0 ? a0 : ( D8 == 1 ? a1 : ( D8 == 2 ? a2 : ( D8 == 3 ? a3 :
                       ( D8 == 4 ? a4 : ( D8 == 5 ? a5 : ( D8 == 6 ? a6 : ( D8 == 7 ? a7 : a8 ))))))))
    , dimension_9<ExN9>( D9 == 0 ? a0 : ( D9 == 1 ? a1 : ( D9 == 2 ? a2 : ( D9 == 3 ? a3 :
                       ( D9 == 4 ? a4 : ( D9 == 5 ? a5 : ( D9 == 6 ? a6 : ( D9 == 7 ? a7 : ( D9 == 8 ? a8 : a9 )))))))))
    {}
};

//----------------------------------------------------------------------------



template< size_t ExN0
        , size_t ExN1
        , size_t ExN2
        , size_t ExN3
        , size_t ExN4
        , size_t ExN5
        , size_t ExN6
        , size_t ExN7
        , size_t ExN8
        , size_t ExN9
        >
inline
std::ostream & operator << ( std::ostream & s , const dimension<ExN0,ExN1,ExN2,ExN3,ExN4,ExN5,ExN6,ExN7,ExN8,ExN9> & dim )
{
  s << "{" ;
  if ( 0 < dim.rank ) s << " " << dim.n0 ;
  if ( 1 < dim.rank ) s << " , " << dim.n1 ;
  if ( 2 < dim.rank ) s << " , " << dim.n2 ;
  if ( 3 < dim.rank ) s << " , " << dim.n3 ;
  if ( 4 < dim.rank ) s << " , " << dim.n4 ;
  if ( 5 < dim.rank ) s << " , " << dim.n5 ;
  if ( 6 < dim.rank ) s << " , " << dim.n6 ;
  if ( 7 < dim.rank ) s << " , " << dim.n7 ;
  if ( 8 < dim.rank ) s << " , " << dim.n8 ;
  if ( 9 < dim.rank ) s << " , " << dim.n9 ;
  s << " }" ;

  return s ;
}

//----------------------------------------------------------------------------

template< class ... Properties >
struct unpack ;

template<>
struct unpack<>
{
  using dimension = void ;
  using layout    = layout_void ;
};

template< class ... Properties >
struct unpack< void , Properties... >
{
  using dimension = typename unpack< Properties... >::dimension ;
  using layout    = typename unpack< Properties... >::layout ;
};

template< class DataType , class ... Properties >
struct unpack< DataType , Properties... >
{
  static constexpr size_t max = std::numeric_limits<size_t>::max();

  // Assume an unknown property is the DataType
  using dimension = std::experimental::view_property::dimension
    < ( 0 < std::rank<DataType>::value ? std::extent<DataType,0>::value : max )
    , ( 1 < std::rank<DataType>::value ? std::extent<DataType,1>::value : max )
    , ( 2 < std::rank<DataType>::value ? std::extent<DataType,2>::value : max )
    , ( 3 < std::rank<DataType>::value ? std::extent<DataType,3>::value : max )
    , ( 4 < std::rank<DataType>::value ? std::extent<DataType,4>::value : max )
    , ( 5 < std::rank<DataType>::value ? std::extent<DataType,5>::value : max )
    , ( 6 < std::rank<DataType>::value ? std::extent<DataType,6>::value : max )
    , ( 7 < std::rank<DataType>::value ? std::extent<DataType,7>::value : max )
    , ( 8 < std::rank<DataType>::value ? std::extent<DataType,8>::value : max )
    , ( 9 < std::rank<DataType>::value ? std::extent<DataType,9>::value : max )
    > ;

  using layout    = typename unpack< Properties... >::layout ;
};

template< size_t ExN0 , size_t ExN1 , size_t ExN2 , size_t ExN3 , size_t ExN4
        , size_t ExN5 , size_t ExN6 , size_t ExN7 , size_t ExN8 , size_t ExN9
        , class ... Properties >
struct unpack< std::experimental::view_property::dimension<ExN0,ExN1,ExN2,ExN3,ExN4,ExN5,ExN6,ExN7,ExN8,ExN9> , Properties... >
{
  using dimension = std::experimental::view_property::dimension<ExN0,ExN1,ExN2,ExN3,ExN4,ExN5,ExN6,ExN7,ExN8,ExN9> ;
  using layout    = typename unpack< Properties ... >::layout ;
};

template< class ... Properties >
struct unpack< layout_left , Properties ... >
{
  using dimension = typename unpack< Properties... >::dimension ;
  using layout    = layout_left ;
};

template< class ... Properties >
struct unpack< layout_right , Properties ... >
{
  using dimension = typename unpack< Properties... >::dimension ;
  using layout    = layout_right ;
};

template< class ... Properties >
struct unpack< layout_stride , Properties ... >
{
  using dimension = typename unpack< Properties... >::dimension ;
  using layout    = layout_stride ;
};

}}} // namespace std::experimental::view_property

//----------------------------------------------------------------------------

namespace std {
namespace experimental {

template< typename DataType , class ... Properties >
struct view 
{
  typedef view_property::unpack< DataType , Properties ... > properties ;

  using value_type = typename std::remove_all_extents< DataType >::type ;
  using pointer    = value_type * ;
  using reference  = value_type & ;
  
private:

  using offset = typename properties::layout::template offset_type<typename properties::dimension>;

  pointer m_ptr ;
  offset  m_offset ;

public:

  static constexpr unsigned rank() { return properties::dimension::rank ; }

  template< typename iType >
  constexpr size_t extent( const iType & i ) const
    {
      return ( i == 0 ? m_offset.dim.n0 :
             ( i == 1 ? m_offset.dim.n1 :
             ( i == 2 ? m_offset.dim.n2 :
             ( i == 3 ? m_offset.dim.n3 :
             ( i == 4 ? m_offset.dim.n4 :
             ( i == 5 ? m_offset.dim.n5 :
             ( i == 6 ? m_offset.dim.n6 :
             ( i == 7 ? m_offset.dim.n7 :
             ( i == 8 ? m_offset.dim.n8 :
             ( i == 9 ? m_offset.dim.n9 : 1 ))))))))));
    }

  constexpr size_t size() const
    { return m_offset.dim.n0 *
             m_offset.dim.n1 *
             m_offset.dim.n2 *
             m_offset.dim.n3 *
             m_offset.dim.n4 *
             m_offset.dim.n5 *
             m_offset.dim.n6 *
             m_offset.dim.n7 *
             m_offset.dim.n8 *
             m_offset.dim.n9 ; }

  using is_regular = typename offset::is_regular ;

  template< typename iType >
  constexpr size_t stride( const iType & i ) const
    {
      return ( i == 0 ? m_offset.dim.stride_0() :
             ( i == 1 ? m_offset.dim.stride_1() :
             ( i == 2 ? m_offset.dim.stride_2() :
             ( i == 3 ? m_offset.dim.stride_3() :
             ( i == 4 ? m_offset.dim.stride_4() :
             ( i == 5 ? m_offset.dim.stride_5() :
             ( i == 6 ? m_offset.dim.stride_6() :
             ( i == 7 ? m_offset.dim.stride_7() :
             ( i == 8 ? m_offset.dim.stride_8() :
             ( i == 9 ? m_offset.dim.stride_9() : 0 ))))))))));
    }

  constexpr size_t span() const { return m_offset.span(); }

  static constexpr
  size_t span
      ( size_t a0 = 0
      , size_t a1 = 0
      , size_t a2 = 0
      , size_t a3 = 0
      , size_t a4 = 0
      , size_t a5 = 0
      , size_t a6 = 0
      , size_t a7 = 0
      , size_t a8 = 0
      , size_t a9 = 0
      )
    { return offset(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9).span(); }

  //----------------------------------------
  // Proper dereference operators for rank 0..3

  struct error_tag_invalid_access_to_non_rank_zero_view
    {
      error_tag_invalid_access_to_non_rank_zero_view( reference ) {}
    };

  typename std::conditional< properties::dimension::rank == 0
                           , reference
                           , error_tag_invalid_access_to_non_rank_zero_view
                           >::type
  operator()() const
    {
      typename std::conditional< properties::dimension::rank == 0
                               , reference
                               , error_tag_invalid_access_to_non_rank_zero_view
                               >::type return_type ;
      return return_type( *m_ptr );
    }

  template< typename t0 >
  typename std::enable_if<( properties::dimension::rank == 1 &&
                            std::is_integral<t0>::value
                         ), reference >::type
  operator[]( const t0 & i0 ) const
    { return m_ptr[ m_offset(i0) ]; }

  template< typename t0 >
  typename std::enable_if<( properties::dimension::rank == 1 &&
                            std::is_integral<t0>::value
                         ), reference >::type
  operator()( const t0 & i0 ) const
    { return m_ptr[ m_offset(i0) ]; }

  template< typename t0 , typename t1 >
  typename std::enable_if<( properties::dimension::rank == 2 &&
                            std::is_integral<t0>::value &&
                            std::is_integral<t1>::value
                         ), reference >::type
  operator()( const t0 & i0 , const t1 & i1 ) const
    { return m_ptr[ m_offset(i0,i1) ]; }

  template< typename t0 , typename t1 , typename t2 >
  typename std::enable_if<( properties::dimension::rank == 3 &&
                            std::is_integral<t0>::value &&
                            std::is_integral<t1>::value &&
                            std::is_integral<t2>::value
                         ), reference >::type
  operator()( const t0 & i0 , const t1 & i1 , const t2 & i2 ) const
    { return m_ptr[ m_offset(i0,i1,i2) ]; }

  //----------------------------------------
  // Improper deference operators for rank 1..3

  template< typename t0 >
  typename std::enable_if<( properties::dimension::rank == 0 &&
                            std::is_integral<t0>::value
                         ), reference >::type
  operator()( const t0 &
            , const int = 0
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            ) const
    { return m_ptr[0]; }

  template< typename t0 >
  typename std::enable_if<( properties::dimension::rank == 1 &&
                            std::is_integral<t0>::value
                         ), reference >::type
  operator()( const t0 & i0 
            , const int
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            , const int = 0 
            ) const
    { return m_ptr[ m_offset(i0) ]; }

  template< typename t0 , typename t1 >
  typename std::enable_if<( properties::dimension::rank == 2 &&
                            std::is_integral<t0>::value &&
                            std::is_integral<t1>::value
                         ), reference >::type
  operator()( const t0 & i0 , const t1 & i1
            , const int
            , const int = 0
            , const int = 0
            , const int = 0
            , const int = 0
            , const int = 0
            , const int = 0
            , const int = 0
            ) const
    { return m_ptr[ m_offset(i0,i1) ]; }

  template< typename t0 , typename t1 , typename t2 >
  typename std::enable_if<( properties::dimension::rank == 3 &&
                            std::is_integral<t0>::value &&
                            std::is_integral<t1>::value &&
                            std::is_integral<t2>::value
                         ), reference >::type
  operator()( const t0 & i0 , const t1 & i1 , const t2 & i2
            , const int
            , const int = 0
            , const int = 0
            , const int = 0
            , const int = 0
            , const int = 0
            , const int = 0
            ) const
    { return m_ptr[ m_offset(i0,i1,i2) ]; }

  //----------------------------------------

  constexpr view() : m_ptr(0), m_offset() {}

  constexpr
  view( pointer ap
      , size_t a0 = 0
      , size_t a1 = 0
      , size_t a2 = 0
      , size_t a3 = 0
      , size_t a4 = 0
      , size_t a5 = 0
      , size_t a6 = 0
      , size_t a7 = 0
      , size_t a8 = 0
      , size_t a9 = 0
      )
    : m_ptr( ap )
    , m_offset(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)
    {}

};

//----------------------------------------------------------------------------

#if ( defined( __GNUC__ ) && ( 4 <= __GNUC__ && __GNUC__ < 5 ) )

template< typename T , unsigned R >
struct implicit_array_type
{
  using type = typename implicit_array_type<T,R-1>::type[] ;
};

template< typename T >
struct implicit_array_type<T,0>
{
  using type = T ;
};

#else

template< typename T , unsigned R >
struct implicit_array_type {
  typedef struct multiple_ommitted_array_dimensions_are_prohibited {} type ;
};

#endif

//----------------------------------------------------------------------------


}} // namespace std::experimental

#endif

