

#include <array_ref>
#include <iostream>

void test_layout_right()
{
  using dim_t = std::experimental::array_property::dimension<10,20,0> ;
  using map_t = std::experimental::array_property::layout_right::mapping< dim_t > ;

  constexpr map_t m( dim_t(30) );

  static_assert( m.span() == 10*20*30 , "" );
  static_assert( m.stride(0) == 20*30 , "" );
  static_assert( m.stride(1) == 30 , "" );
  static_assert( m.stride(2) == 1 , "" );

}


int main()
{
  test_layout_right();
  return 0 ;
}

