
#include <view.hpp>

#include <iostream>
#include <typeinfo>

int main()
{
  using namespace std::experimental ;

  std::cout << " sizeof(dimension<>) = " << sizeof(dimension<>) << std::endl ;
  std::cout << " sizeof(dimension<1>) = " << sizeof(dimension<1>) << std::endl ;
  std::cout << " sizeof(dimension<0>) = " << sizeof(dimension<0>) << std::endl ;
  std::cout << " sizeof(dimension<0,0>) = " << sizeof(dimension<0,0>) << std::endl ;
  std::cout << " sizeof(dimension<0,0,0>) = " << sizeof(dimension<0,0,0>) << std::endl ;
  std::cout << " sizeof(dimension<1,0,0>) = " << sizeof(dimension<1,0,0>) << std::endl ;

  std::cout << dimension<>() << std::endl ;
  std::cout << dimension<1,2,3,4,5,6,7,8,9,10>() << std::endl ;
  std::cout << dimension<0,2,3,4,5,6,7,8,9,10>(11) << std::endl ;
  std::cout << dimension<1,0,3,4,5,6,7,8,9,10>(12) << std::endl ;
  std::cout << dimension<1,2,0,4,5,6,7,8,9,10>(13) << std::endl ;
  std::cout << dimension<1,2,3,0,5,6,7,8,9,10>(14) << std::endl ;
  std::cout << dimension<1,2,3,4,0,6,7,8,9,10>(15) << std::endl ;
  std::cout << dimension<0,2,3,4,0,6,7,8,9,10>(11,15) << std::endl ;
  std::cout << dimension<0,2,3,4,0>(11,15) << std::endl ;

  typedef implicit_array_type<int,0>::type t0 ;
  typedef implicit_array_type<int,1>::type t1 ;
  typedef implicit_array_type<int,2>::type t2 ;

  std::cout << typeid(t0).name() << std::endl ;
  std::cout << typeid(t1).name() << std::endl ;
  std::cout << typeid(t2).name() << std::endl ;

  view< int[] > x ;

  return 0 ;
}

