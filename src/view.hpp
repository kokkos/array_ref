
#ifndef STD_EXPERIMENTAL_VIEW
#define STD_EXPERIMENTAL_VIEW

#include <limits>
#include <ostream>

namespace std {
namespace experimental {

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

}}

#endif

