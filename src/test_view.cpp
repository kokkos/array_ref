
#include <view.hpp>

#include <iostream>
#include <typeinfo>
#include <cassert>

int main()
{
  using namespace std::experimental ;
  using namespace std::experimental::view_property ;

  // Verify only implicit dimensions have variables.
  std::cout << " sizeof(dimension<>) = " << sizeof(dimension<>) << std::endl ;
  std::cout << " sizeof(dimension<1>) = " << sizeof(dimension<1>) << std::endl ;
  std::cout << " sizeof(dimension<0>) = " << sizeof(dimension<0>) << std::endl ;
  std::cout << " sizeof(dimension<0,0>) = " << sizeof(dimension<0,0>) << std::endl ;
  std::cout << " sizeof(dimension<0,0,0>) = " << sizeof(dimension<0,0,0>) << std::endl ;
  std::cout << " sizeof(dimension<1,0,0>) = " << sizeof(dimension<1,0,0>) << std::endl ;

  // Verify dimension unpacking:
  std::cout << dimension<>() << std::endl ;
  std::cout << dimension<1,2,3,4,5,6,7,8,9,10>() << std::endl ;
  std::cout << dimension<0,2,3,4,5,6,7,8,9,10>(11) << std::endl ;
  std::cout << dimension<1,0,3,4,5,6,7,8,9,10>(12) << std::endl ;
  std::cout << dimension<1,2,0,4,5,6,7,8,9,10>(13) << std::endl ;
  std::cout << dimension<1,2,3,0,5,6,7,8,9,10>(14) << std::endl ;
  std::cout << dimension<1,2,3,4,0,6,7,8,9,10>(15) << std::endl ;
  std::cout << dimension<0,2,3,4,0,6,7,8,9,10>(11,15) << std::endl ;
  std::cout << dimension<0,2,3,4,0>(11,15) << std::endl ;

  // Can construct 'int[][][]' type:
  typedef implicit_array_type<int,0>::type t0 ;
  typedef implicit_array_type<int,1>::type t1 ;
  typedef implicit_array_type<int,2>::type t2 ;

  std::cout << typeid(t0).name() << std::endl ;
  std::cout << typeid(t1).name() << std::endl ;
  std::cout << typeid(t2).name() << std::endl ;

  // Verify extent can be evaluated at compilation
  // for 'constexpr' constructed view.
  static_assert( view<int, layout_offset_void<dimension<0,4> > >().extent(0) == 0 , "" );
  static_assert( view<int, layout_offset_left<dimension<3,10> > >().extent(1) == 10 , "" );
  static_assert( view<int, layout_offset_right<dimension<3,0,1> > >().extent(2) == 1 , "" );

  // Verify view construction
  {
    int x_data[100];
    view< int, layout_offset_right<dimension<0,10> > > x(x_data,9);
    static_assert( x.rank() == 2 , "" );
    assert( x.extent(0) == 9 );
    static_assert( x.extent(1) == 10 , "" );
    // x(5) = 41 ; // no match due to insufficient rank
    x(5,5) = 42 ;   // matches proper rank
    x(5,5,0) = 43 ; // matches improper rank
  }

  return 0 ;
}

