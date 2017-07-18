
#include <assert.h>
#include <iostream>
#include <mdspan>

int main()
{
  using namespace std::experimental ;

  {
    constexpr extents<2,4,8> d3 ;
  
    static_assert( d3.rank() == 3 , "" );
    static_assert( d3.rank_dynamic() == 0 , "" );
    static_assert( d3.static_extent(0) == 2 , "" );
    static_assert( d3.static_extent(1) == 4 , "" );
    static_assert( d3.static_extent(2) == 8 , "" );
    static_assert( d3.extent(0) == 2 , "" );
    static_assert( d3.extent(1) == 4 , "" );
    static_assert( d3.extent(2) == 8 , "" );
    static_assert( d3.size() == 2 * 4 * 8 , "" );
  }

  {
    constexpr extents<2,dynamic_extent,8,dynamic_extent> d4 ;
  
    static_assert( d4.rank() == 4 , "" );
    static_assert( d4.rank_dynamic() == 2 , "" );
    static_assert( d4.static_extent(0) == 2 , "" );
    static_assert( d4.static_extent(1) == dynamic_extent , "" );
    static_assert( d4.static_extent(2) == 8 , "" );
    static_assert( d4.static_extent(3) == dynamic_extent , "" );
    static_assert( d4.extent(0) == 2 , "" );
    static_assert( d4.extent(1) == 0 , "" );
    static_assert( d4.extent(2) == 8 , "" );
    static_assert( d4.extent(3) == 0 , "" );
    static_assert( d4.size() == 0 , "" );
  }

  {
    constexpr extents<2,dynamic_extent,8,dynamic_extent,32> d5(4,16);
  
    static_assert( d5.rank() == 5 , "" );
    static_assert( d5.rank_dynamic() == 2 , "" );
    static_assert( d5.static_extent(0) == 2 , "" );
    static_assert( d5.static_extent(1) == dynamic_extent , "" );
    static_assert( d5.static_extent(2) == 8 , "" );
    static_assert( d5.static_extent(3) == dynamic_extent , "" );
    static_assert( d5.static_extent(4) == 32 , "" );
    static_assert( d5.extent(0) == 2 , "" );
    static_assert( d5.extent(1) == 4 , "" );
    static_assert( d5.extent(2) == 8 , "" );
    static_assert( d5.extent(3) == 16 , "" );
    static_assert( d5.extent(4) == 32 , "" );
    static_assert( d5.size() == 2 * 4 * 8 * 16 * 32 , "" );
  }

  {
    constexpr layout_right::mapping<2,4,8> m3 ;

    static_assert( m3.rank() == 3 , "" );
    static_assert( m3.rank_dynamic() == 0 , "" );
    static_assert( m3.static_extent(0) == 2 , "" );
    static_assert( m3.static_extent(1) == 4 , "" );
    static_assert( m3.static_extent(2) == 8 , "" );
    static_assert( m3.extent(0) == 2 , "" );
    static_assert( m3.extent(1) == 4 , "" );
    static_assert( m3.extent(2) == 8 , "" );
    static_assert( m3.size() == 2 * 4 * 8 , "" );

    static_assert( m3() == 0 , "" );
    static_assert( m3(0,0,0) == 0 , "" );
    static_assert( m3(0,0,1) == 1 , "" );
    static_assert( m3(1,1,1) == ( ( ( ( 1 ) * 4 ) + 1 ) * 8 + 1 ) , "" );
    static_assert( m3(1,2,3) == ( ( ( ( 1 ) * 4 ) + 2 ) * 8 + 3 ) , "" );
  }

  {
    constexpr int N0 = 2 ;
    constexpr int N1 = 4 ;
    constexpr int N2 = 8 ;
    constexpr int N3 = 16 ;
    constexpr int N4 = 32 ;

    constexpr layout_right::mapping<N0,dynamic_extent,N2,dynamic_extent,N4>
      m5(N1,N3);

    static_assert( m5.rank() == 5 , "" );
    static_assert( m5.rank_dynamic() == 2 , "" );
    static_assert( m5.static_extent(0) == N0 , "" );
    static_assert( m5.static_extent(1) == dynamic_extent , "" );
    static_assert( m5.static_extent(2) == N2 , "" );
    static_assert( m5.static_extent(3) == dynamic_extent , "" );
    static_assert( m5.static_extent(4) == N4 , "" );
    static_assert( m5.static_extent(5) == 1 , "" );
    static_assert( m5.extent(0) == N0 , "" );
    static_assert( m5.extent(1) == N1 , "" );
    static_assert( m5.extent(2) == N2 , "" );
    static_assert( m5.extent(3) == N3 , "" );
    static_assert( m5.extent(4) == N4 , "" );
    static_assert( m5.extent(5) == 1 , "" );
    static_assert( m5.size() == N0 * N1 * N2 * N3 * N4 , "" );
    static_assert( m5.stride(0) == N1 * N2 * N3 * N4 , "" );
    static_assert( m5.stride(1) == N2 * N3 * N4 , "" );
    static_assert( m5.stride(2) == N3 * N4 , "" );
    static_assert( m5.stride(3) == N4 , "" );
    static_assert( m5.stride(4) == 1 , "" );
    static_assert( m5.stride(5) == 1 , "" );

    static_assert( m5(1,2,3,4,5) == ((((((1)*N1)+2)*N2+3)*N3+4)*N4+5) , "" );
    static_assert( m5(1,2,3,4,5,0) == ((((((1)*N1)+2)*N2+3)*N3+4)*N4+5) , "" );
  }

  {
    constexpr int N0 = 2 ;
    constexpr int N1 = 4 ;
    constexpr int N2 = 8 ;
    constexpr int N3 = 16 ;
    constexpr int N4 = 32 ;

    constexpr layout_left::mapping<N0,dynamic_extent,N2,dynamic_extent,N4>
      m5(N1,N3);

    static_assert( m5.rank() == 5 , "" );
    static_assert( m5.rank_dynamic() == 2 , "" );
    static_assert( m5.static_extent(0) == N0 , "" );
    static_assert( m5.static_extent(1) == dynamic_extent , "" );
    static_assert( m5.static_extent(2) == N2 , "" );
    static_assert( m5.static_extent(3) == dynamic_extent , "" );
    static_assert( m5.static_extent(4) == N4 , "" );
    static_assert( m5.static_extent(5) == 1 , "" );
    static_assert( m5.extent(0) == N0 , "" );
    static_assert( m5.extent(1) == N1 , "" );
    static_assert( m5.extent(2) == N2 , "" );
    static_assert( m5.extent(3) == N3 , "" );
    static_assert( m5.extent(4) == N4 , "" );
    static_assert( m5.extent(5) == 1 , "" );
    static_assert( m5.size() == N0 * N1 * N2 * N3 * N4 , "" );
    static_assert( m5.stride(0) == 1 , "" );
    static_assert( m5.stride(1) == N0 , "" );
    static_assert( m5.stride(2) == N0 * N1 , "" );
    static_assert( m5.stride(3) == N0 * N1 * N2 , "" );
    static_assert( m5.stride(4) == N0 * N1 * N2 * N3 , "" );
    static_assert( m5.stride(5) == N0 * N1 * N2 * N3 * N4 , "" );

    static_assert( m5(1,2,3,4,5) == (1+N0*(2+N1*(3+N2*(4+N3*(5))))) , "" );
    static_assert( m5(1,2,3,4,5,0) == (1+N0*(2+N1*(3+N2*(4+N3*(5))))) , "" );
  }

  {
    constexpr int N0 = 2 ;
    constexpr int N1 = 4 ;
    constexpr int N2 = 8 ;
    constexpr int N3 = 16 ;
    constexpr int N4 = 32 ;

    constexpr int S0 = 1 ;
    constexpr int S1 = 1 + S0 * N0 ;
    constexpr int S2 = 1 + S1 * N1 ;
    constexpr int S3 = 1 + S2 * N2 ;
    constexpr int S4 = 1 + S3 * N3 ;

    const int DynamicExtents[] = { N1 , N3 };
    const int Strides[] = { S0 , S1 , S2 , S3 , S4 };

    layout_stride::mapping<N0,dynamic_extent,N2,dynamic_extent,N4>
      m5( DynamicExtents , Strides );

    static_assert( m5.rank() == 5 , "" );
    static_assert( m5.rank_dynamic() == 2 , "" );
    static_assert( m5.static_extent(0) == N0 , "" );
    static_assert( m5.static_extent(1) == dynamic_extent , "" );
    static_assert( m5.static_extent(2) == N2 , "" );
    static_assert( m5.static_extent(3) == dynamic_extent , "" );
    static_assert( m5.static_extent(4) == N4 , "" );
    static_assert( m5.static_extent(5) == 1 , "" );

    assert( m5.extent(0) == N0 );
    assert( m5.extent(1) == N1 );
    assert( m5.extent(2) == N2 );
    assert( m5.extent(3) == N3 );
    assert( m5.extent(4) == N4 );
    assert( m5.extent(5) == 1 );
    assert( m5.size() == N0 * N1 * N2 * N3 * N4 );

    assert( m5.stride(0) == Strides[0] );
    assert( m5.stride(1) == Strides[1] );
    assert( m5.stride(2) == Strides[2] );
    assert( m5.stride(3) == Strides[3] );
    assert( m5.stride(4) == Strides[4] );
    assert( m5.stride(5) == 0 );

    assert( m5.is_unique() );
    assert( ! m5.is_contiguous() );

    assert( m5(1,2,3,4,5) == 1 * Strides[0] +
                             2 * Strides[1] +
                             3 * Strides[2] +
                             4 * Strides[3] +
                             5 * Strides[4] );

    assert( m5(1,2,3,4,5,0) == 1 * Strides[0] +
                               2 * Strides[1] +
                               3 * Strides[2] +
                               4 * Strides[3] +
                               5 * Strides[4] );
  }

  {
    constexpr int N0 = 2 ;
    constexpr int N2 = 8 ;
    constexpr int N4 = 32 ;

    typedef extents<N0,dynamic_extent,N2,dynamic_extent,N4> e5 ;

    typedef typename
      detail::sub_extents_deduction< e5, int, int, int, int, int >::type e0 ;
    typedef typename
      detail::sub_extents_deduction< e5, int, all_type, int, int, int >::type e1 ;
    typedef typename
      detail::sub_extents_deduction< e5, int, all_type, all_type, int, int >::type e2 ;
  
    typedef typename
      detail::sub_extents_deduction< e5, int, all_type, int, all_type, all_type >::type e3 ;
  
    typedef typename
      detail::sub_extents_deduction< e5, all_type, int, all_type, all_type, all_type >::type e4 ;
  
    typedef typename
      detail::sub_extents_deduction< e5, all_type, int, std::pair<int,int>, all_type, all_type >::type e4A ;
  
    static_assert( e0::rank() == 0 , "" );
    static_assert( e0::rank_dynamic() == 0 , "" );

    static_assert( e1::rank() == 1 , "" );
    static_assert( e1::rank_dynamic() == 1 , "" );
    static_assert( e1::static_extent(0) == dynamic_extent , "" );

    static_assert( e2::rank() == 2 , "" );
    static_assert( e2::rank_dynamic() == 1 , "" );
    static_assert( e2::static_extent(0) == dynamic_extent , "" );
    static_assert( e2::static_extent(1) == N2 , "" );

    static_assert( e3::rank() == 3 , "" );
    static_assert( e3::rank_dynamic() == 2 , "" );
    static_assert( e3::static_extent(0) == dynamic_extent , "" );
    static_assert( e3::static_extent(1) == dynamic_extent , "" );
    static_assert( e3::static_extent(2) == N4 , "" );

    static_assert( e4::rank() == 4 , "" );
    static_assert( e4::rank_dynamic() == 1 , "" );
    static_assert( e4::static_extent(0) == N0 , "" );
    static_assert( e4::static_extent(1) == N2 , "" );
    static_assert( e4::static_extent(2) == dynamic_extent , "" );
    static_assert( e4::static_extent(3) == N4 , "" );

    static_assert( e4A::rank() == 4 , "" );
    static_assert( e4A::rank_dynamic() == 2 , "" );
    static_assert( e4A::static_extent(0) == N0 , "" );
    static_assert( e4A::static_extent(1) == dynamic_extent , "" );
    static_assert( e4A::static_extent(2) == dynamic_extent , "" );
    static_assert( e4A::static_extent(3) == N4 , "" );
  }

  {
    constexpr int N0 = 2 ;
    constexpr int N1 = 4 ;
    constexpr int N2 = 8 ;
    constexpr int N3 = 16 ;
    constexpr int N4 = 32 ;

    int buffer[ N0 * N1 * N2 * N3 * N4 ];

    typedef extents<N0,dynamic_extent,N2,dynamic_extent,N4> ex5 ;

    mdspan<int,ex5> m5(buffer,N1,N3);

    assert( m5.rank() == 5 );
    assert( m5.rank_dynamic() == 2 );
    assert( m5.static_extent(0) == N0 );
    assert( m5.static_extent(1) == dynamic_extent );
    assert( m5.static_extent(2) == N2 );
    assert( m5.static_extent(3) == dynamic_extent );
    assert( m5.static_extent(4) == N4 );
    assert( m5.extent(0) == N0 );
    assert( m5.extent(1) == N1 );
    assert( m5.extent(2) == N2 );
    assert( m5.extent(3) == N3 );
    assert( m5.extent(4) == N4 );
    assert( m5.size() == N0 * N1 * N2 * N3 * N4 );

    assert( & m5(1,2,3,4,5) == & buffer[((((((1)*N1)+2)*N2+3)*N3+4)*N4+5)] );
    assert( & m5(1,2,3,4,5,0) == & buffer[((((((1)*N1)+2)*N2+3)*N3+4)*N4+5)] );

    mdspan<const int,ex5> ok(m5);
    // mdspan<double,ex5> err(m5);

    auto x = subspan( m5 , all , 0 , 0 , all , 0 );

    assert( x.rank() == 2 );
    assert( x.rank_dynamic() == 1 );
    assert( x.static_extent(0) == N0 );
    assert( x.static_extent(1) == dynamic_extent );
    assert( x.extent(0) == N0 );
    assert( x.extent(1) == N3 );
    assert( x.stride(0) == m5.stride(0) );
    assert( x.stride(1) == m5.stride(3) );
    assert( x.is_unique() );
    assert( ! x.is_contiguous() );
    assert( x.span_size() == 1 + m5.offset(N0-1,0,0,N3-1,0) );

    assert( & m5(0,0,0,0,0) == & x(0,0) );
    assert( & m5(1,0,0,2,0) == & x(1,2) );
    assert( & m5(N0-1,0,0,N3-1,0) == & x(N0-1,N3-1) );
  }

  return 0 ;
}

