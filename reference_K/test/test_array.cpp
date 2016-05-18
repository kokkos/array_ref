
#include <assert.h>

#include <array_ref>
#include <iostream>

void test_array_base()
{
  using array_t = std::experimental::array_ref<int[][20][30][40]> ;

  static_assert( array_t::is_always_unique , "" );
  static_assert( array_t::is_always_regular , "" );
  static_assert( array_t::is_always_contiguous , "" );
  static_assert( array_t::rank() == 4 , "" );
  static_assert( array_t::rank_dynamic() == 1 , "" );

  static_assert( std::is_same< array_t::layout , std::experimental::array_property::layout_right >::value , "" );
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

}


int main()
{
  test_array_base();
  return 0 ;
}

