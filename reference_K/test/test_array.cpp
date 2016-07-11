
#include <assert.h>

#include <array_ref>
#include <iostream>

void test_array_base()
{
  using array_t = std::experimental::array_ref<int[][20][30][40]> ;
  using const_array_t = std::experimental::array_ref<const int[][20][30][40]> ;

  static_assert( array_t::is_always_unique , "" );
  static_assert( array_t::is_always_regular , "" );
  static_assert( array_t::is_always_contiguous , "" );
  static_assert( array_t::rank() == 4 , "" );
  static_assert( array_t::rank_dynamic() == 1 , "" );

  static_assert( std::is_same< array_t::layout , std::experimental::layout_right >::value , "" );
  static_assert( std::is_same< array_t::size_type , size_t >::value , "" );
  static_assert( std::is_same< array_t::value_type , int >::value , "" );
  static_assert( std::is_same< array_t::reference , int & >::value , "" );
  static_assert( std::is_same< array_t::pointer , int * >::value , "" );

  constexpr array_t a ;

  static_assert( a.is_unique() , "" );
  static_assert( a.is_regular() , "" );
  static_assert( a.is_contiguous() , "" );
  static_assert( a.size() == 0 , "" );
  static_assert( a.extent(0) == 0 , "" );
  static_assert( a.extent(1) == 20 , "" );
  static_assert( a.extent(2) == 30 , "" );
  static_assert( a.extent(3) == 40 , "" );

  static_assert( a.span() == 0 , "" );
  static_assert( a.stride(0) == 20*30*40 , "" );
  static_assert( a.stride(1) == 30*40 , "" );
  static_assert( a.stride(2) == 40 , "" );
  static_assert( a.stride(3) == 1 , "" );

  static_assert( a.data() == 0 , "" );

  int buffer[10*20*30*40];

  array_t b(buffer,10);

  assert( b.span() == 10*20*30*40 );
  assert( b.stride(0) == 20*30*40 );
  assert( b.stride(1) == 30*40 );
  assert( b.stride(2) == 40 );
  assert( b.stride(3) == 1 );

  assert( b.data() == buffer );

  assert( & b(0,0,0,0) == buffer );
  assert( & b(1,0,0,0) == buffer + 1*20*30*40 );
  assert( & b(0,1,0,0) == buffer + 1*30*40 );
  assert( & b(0,0,1,0) == buffer + 1*40 );
  assert( & b(0,0,0,1) == buffer + 1 );

  assert( b.data()  == buffer );
  assert( b.data() + b.span()   == buffer + 10*20*30*40 );

  const_array_t c( b );

  assert( & c(0,0,0,0) == buffer );
  assert( & c(1,0,0,0) == buffer + 1*20*30*40 );
  assert( & c(0,1,0,0) == buffer + 1*30*40 );
  assert( & c(0,0,1,0) == buffer + 1*40 );
  assert( & c(0,0,0,1) == buffer + 1 );
}

void test_array_strided()
{
  using array_t = std::experimental::array_ref<int[][20][30][40]> ;
  using array_strided_t = std::experimental::array_ref<int, std::experimental::extents<0,0,0,0> , std::experimental::layout_stride > ;

  int buffer[10*20*30*40];

  array_t b(buffer,10);

  array_strided_t s( b );

  assert( s.is_regular() == true );
  assert( s.is_unique() == true );
  assert( s.is_contiguous() == true );
  assert( s.extent(0) == b.extent(0) );
  assert( s.extent(1) == b.extent(1) );
  assert( s.extent(2) == b.extent(2) );
  assert( s.extent(3) == b.extent(3) );
  assert( s.stride(0) == b.stride(0) );
  assert( s.stride(1) == b.stride(1) );
  assert( s.stride(2) == b.stride(2) );
  assert( s.stride(3) == b.stride(3) );

  assert( & s(1,1,1,1) == & b(1,1,1,1) );
}

void test_array_subarray()
{
  using array_t = std::experimental::array_ref<int[][20][30][40]> ;
  using range_t = std::pair<size_t,size_t> ;
  auto all = std::experimental::all ;

  int buffer[10*20*30*40];

  array_t b(buffer,10);

  auto a = std::experimental::subarray( b , all , all , all , all );
  auto c = std::experimental::subarray( b , 1 , 1 , 1 , 1 );
  auto d = std::experimental::subarray( b , range_t(1,2), range_t(1,3), range_t(1,4), range_t(1,5) );

  // bug in gcc does not produce initializer list for template arguments
  // auto e = std::experimental::subarray( b , {1,2} , {1,3} , {1,4} , {1,5} );

  static_assert( a.rank() == 4 , "" );
  assert( a.extent(0) == b.extent(0) );
  assert( a.extent(1) == b.extent(1) );
  assert( a.extent(2) == b.extent(2) );
  assert( a.extent(3) == b.extent(3) );
  assert( a.stride(0) == b.stride(0) );
  assert( a.stride(1) == b.stride(1) );
  assert( a.stride(2) == b.stride(2) );
  assert( a.stride(3) == b.stride(3) );
  assert( a.is_regular() == true );
  assert( a.is_unique() == true );
  assert( a.is_contiguous() == true );

  static_assert( c.rank() == 0 , "" );
  static_assert( d.rank() == 4 , "" );

  assert( & b(1,1,1,1) == & c() );

  assert( d.extent(0) == 1 );
  assert( d.extent(1) == 2 );
  assert( d.extent(2) == 3 );
  assert( d.extent(3) == 4 );

  assert( d.stride(0) == b.stride(0) );
  assert( d.stride(1) == b.stride(1) );
  assert( d.stride(2) == b.stride(2) );
  assert( d.stride(3) == b.stride(3) );
  assert( & b(1,1,1,1) == & d(0,0,0,0) );
  assert( & b(1,2,3,4) == & d(0,1,2,3) );

  assert( d.is_regular() == true );
  assert( d.is_unique() == true );
  assert( d.is_contiguous() == false );

  // Subarray of subarray, dropping a rank
  auto dd = std::experimental::subarray( d , 1 , range_t(1,2) , range_t(1,3) , range_t(1,4) );

  assert( dd.is_regular() == true );
  assert( dd.is_unique() == true );
  assert( dd.is_contiguous() == false );
  static_assert( dd.rank() == 3 , "");
  assert( dd.extent(0) == 1 );
  assert( dd.extent(1) == 2 );
  assert( dd.extent(2) == 3 );
  assert( dd.stride(0) == b.stride(1) );
  assert( dd.stride(1) == b.stride(2) );
  assert( dd.stride(2) == b.stride(3) );
  assert( & d(1,1,1,1) == & dd(0,0,0) );
  assert( & d(1,1,2,3) == & dd(0,1,2) );
}


int main()
{
  test_array_base();
  test_array_subarray();

  return 0 ;
}

