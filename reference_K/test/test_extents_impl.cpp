
#include <array_ref>
#include <iostream>

constexpr size_t generate_extent( unsigned r )
  { return static_cast<size_t>(2) * ( r + 1 ); }

template< typename ExtentType
        , unsigned RankStatic
        , unsigned RankDynamic
        , unsigned R = 0 >
struct generate_extents_impl {

  using type = typename std::conditional
    < ( RankStatic < RankDynamic )
    , typename std::experimental::detail::prepend_extents
         < typename generate_extents_impl<ExtentType,RankStatic,(RankDynamic?RankDynamic-1:0),R+1>::type , 0 >::type
    , typename std::experimental::detail::prepend_extents
         < typename generate_extents_impl<ExtentType,(RankStatic?RankStatic-1:0),RankDynamic,R+1>::type , generate_extent(R) > ::type
    >::type ;
};

template< typename ExtentType , unsigned R >
struct generate_extents_impl< ExtentType , 0 , 0 , R >
{
  using type = std::experimental::detail::extents_impl<0,ExtentType> ;
};

template< typename ExtentType , unsigned RankStatic , unsigned R >
struct generate_extents_impl< ExtentType , RankStatic , 0 , R >
{
  using nest = typename generate_extents_impl<ExtentType,RankStatic-1,0,R+1>::type ;
  using type = typename std::experimental::detail::prepend_extents< nest , generate_extent(R) >::type ;
};

template< typename ExtentType , unsigned RankDynamic , unsigned R >
struct generate_extents_impl< ExtentType , 0 , RankDynamic , R >
{
  using nest = typename generate_extents_impl<ExtentType,0,RankDynamic-1,R+1>::type ;
  using type = typename std::experimental::detail::prepend_extents< nest , 0 >::type ;
};

void test_extents_constexpr()
{
  using dim_size_t_00_00_t = typename generate_extents_impl<size_t,0,0>::type ;
  using dim_int_00_00_t    = typename generate_extents_impl<int,0,0>::type ;
  using dim_int_01_00_t    = typename generate_extents_impl<int,1,0>::type ;
  using dim_int_00_01_t    = typename generate_extents_impl<int,0,1>::type ;
  using dim_int_01_01_t    = typename generate_extents_impl<int,1,1>::type ;
  using dim_int_07_00_t    = typename generate_extents_impl<int,7,0>::type ;
  using dim_int_00_07_t    = typename generate_extents_impl<int,0,7>::type ;
  using dim_int_07_07_t    = typename generate_extents_impl<int,7,7>::type ;

  static_assert( dim_size_t_00_00_t::rank() == 0 , "" );
  static_assert( dim_int_00_00_t::rank() == 0 , "" );
  static_assert( dim_int_01_00_t::rank() == 1 , "" );
  static_assert( dim_int_00_01_t::rank() == 1 , "" );
  static_assert( dim_int_01_01_t::rank() == 2 , "" );
  static_assert( dim_int_07_00_t::rank() == 7 , "" );
  static_assert( dim_int_00_07_t::rank() == 7 , "" );
  static_assert( dim_int_07_07_t::rank() == 14 , "" );

  static_assert( dim_size_t_00_00_t::rank_dynamic() == 0 , "" );
  static_assert( dim_int_00_00_t::rank_dynamic() == 0 , "" );
  static_assert( dim_int_01_00_t::rank_dynamic() == 0 , "" );
  static_assert( dim_int_00_01_t::rank_dynamic() == 1 , "" );
  static_assert( dim_int_01_01_t::rank_dynamic() == 1 , "" );
  static_assert( dim_int_07_00_t::rank_dynamic() == 0 , "" );
  static_assert( dim_int_00_07_t::rank_dynamic() == 7 , "" );
  static_assert( dim_int_07_07_t::rank_dynamic() == 7 , "" );

  static_assert( sizeof(dim_size_t_00_00_t) <= sizeof(size_t) , "" );
  static_assert( sizeof(dim_int_00_00_t) <= sizeof(int) , "" );
  static_assert( sizeof(dim_int_01_00_t) <= sizeof(int) , "" );
  static_assert( sizeof(dim_int_00_01_t) == sizeof(int) , "" );
  static_assert( sizeof(dim_int_01_01_t) == sizeof(int) , "" );
  static_assert( sizeof(dim_int_07_00_t) <= sizeof(int) , "" );
  static_assert( sizeof(dim_int_00_07_t) == 7 * sizeof(int) , "" );
  static_assert( sizeof(dim_int_07_07_t) == 7 * sizeof(int) , "" );

  constexpr dim_int_00_00_t dim_int_00_00 ;
  constexpr dim_int_01_00_t dim_int_01_00 ;
  constexpr dim_int_00_01_t dim_int_00_01 ;
  constexpr dim_int_01_01_t dim_int_01_01 ;
  constexpr dim_int_07_00_t dim_int_07_00 ;
  constexpr dim_int_00_07_t dim_int_00_07 ;
  constexpr dim_int_07_07_t dim_int_07_07 ;

  static_assert( dim_int_00_00.rank() == 0 , "" );
  static_assert( dim_int_00_00.rank_dynamic() == 0 , "" );
  static_assert( dim_int_00_00.static_extent<0>() == 1 , "" );
  static_assert( dim_int_00_00.static_extent(0) == 1 , "" );
  static_assert( dim_int_00_00.extent<0>() == 1 , "" );
  static_assert( dim_int_00_00.extent(0) == 1 , "" );
  static_assert( dim_int_00_00.static_extent<1>() == 1 , "" );
  static_assert( dim_int_00_00.static_extent(1) == 1 , "" );
  static_assert( dim_int_00_00.extent<1>() == 1 , "" );
  static_assert( dim_int_00_00.extent(1) == 1 , "" );

  static_assert( dim_int_01_00.extent(0) == (2*1) , "" );
  static_assert( dim_int_00_01.extent(0) == 0 , "" );
  static_assert( dim_int_01_01.extent(0) == (2*1) , "" );
  static_assert( dim_int_01_01.extent(1) == 0, "" );
  static_assert( dim_int_01_00.size() == (2*1) , "" );
  static_assert( dim_int_00_01.size() == 0 , "" );
  static_assert( dim_int_01_01.size() == 0 , "" );

  static_assert( dim_int_07_00.extent(0) == (2*1) , "" );
  static_assert( dim_int_07_00.extent(1) == (2*2) , "" );
  static_assert( dim_int_07_00.extent(2) == (2*3) , "" );
  static_assert( dim_int_07_00.extent(3) == (2*4) , "" );
  static_assert( dim_int_07_00.extent(4) == (2*5) , "" );
  static_assert( dim_int_07_00.extent(5) == (2*6) , "" );
  static_assert( dim_int_07_00.extent(6) == (2*7) , "" );
  static_assert( dim_int_07_00.size() == (2*1)*(2*2)*(2*3)*(2*4)*(2*5)*(2*6)*(2*7) , "" );

  static_assert( dim_int_07_07.extent(0) == (2*1) , "" );
  static_assert( dim_int_07_07.extent(1) == 0 , "" );
  static_assert( dim_int_07_07.extent(2) == (2*3) , "" );
  static_assert( dim_int_07_07.extent(3) == 0 , "" );
  static_assert( dim_int_07_07.extent(4) == (2*5) , "" );
  static_assert( dim_int_07_07.extent(5) == 0 , "" );
  static_assert( dim_int_07_07.extent(6) == (2*7) , "" );
  static_assert( dim_int_07_07.extent(7) == 0 , "" );
  static_assert( dim_int_07_07.extent(8) == (2*9) , "" );
  static_assert( dim_int_07_07.extent(9) == 0 , "" );
  static_assert( dim_int_07_07.extent(10) == (2*11) , "" );
  static_assert( dim_int_07_07.extent(11) == 0 , "" );
  static_assert( dim_int_07_07.extent(12) == (2*13) , "" );
  static_assert( dim_int_07_07.extent(13) == 0 , "" );

  constexpr dim_int_00_01_t dim_int_00_01_value(1*2);
  constexpr dim_int_01_01_t dim_int_01_01_value(2*2);

  constexpr dim_int_07_07_t dim_int_07_07_value( 2*2 , 2*4 , 2*6 , 2*8 , 2*10 , 2*12 , 2*14 );

  static_assert( dim_int_00_01_value.extent(0) == (2*1) , "" );
  static_assert( dim_int_01_01_value.extent(0) == (2*1) , "" );
  static_assert( dim_int_01_01_value.extent(1) == (2*2), "" );
  static_assert( dim_int_00_01_value.size() == (2*1) , "" );
  static_assert( dim_int_01_01_value.size() == (2*1)*(2*2) , "" );

  static_assert( dim_int_07_07_value.extent(0) == (2*1) , "" );
  static_assert( dim_int_07_07_value.extent(1) == (2*2) , "" );
  static_assert( dim_int_07_07_value.extent(2) == (2*3) , "" );
  static_assert( dim_int_07_07_value.extent(3) == (2*4) , "" );
  static_assert( dim_int_07_07_value.extent(4) == (2*5) , "" );
  static_assert( dim_int_07_07_value.extent(5) == (2*6) , "" );
  static_assert( dim_int_07_07_value.extent(6) == (2*7) , "" );
  static_assert( dim_int_07_07_value.extent(7) == (2*8) , "" );
  static_assert( dim_int_07_07_value.extent(8) == (2*9) , "" );
  static_assert( dim_int_07_07_value.extent(9) == (2*10) , "" );
  static_assert( dim_int_07_07_value.extent(10) == (2*11) , "" );
  static_assert( dim_int_07_07_value.extent(11) == (2*12) , "" );
  static_assert( dim_int_07_07_value.extent(12) == (2*13) , "" );
  static_assert( dim_int_07_07_value.extent(13) == (2*14) , "" );

  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<0>() == (2*1) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<1>() == (2*2) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<2>() == (2*3) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<3>() == (2*4) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<4>() == (2*5) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<5>() == (2*6) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<6>() == (2*7) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<7>() == (2*8) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<8>() == (2*9) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<9>() == (2*10) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<10>() == (2*11) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<11>() == (2*12) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<12>() == (2*13) , "" );
  static_assert( dim_int_07_07_value.dim_int_07_07_t::extent<13>() == (2*14) , "" );

  static_assert( dim_int_07_07_value.in_bounds(0,1,2,3,4,5,6,7,8,9,10,11,12,13) , "" );
  static_assert( ! dim_int_07_07_value.in_bounds(0,1,2,3,4,5,6,7,8,9,10,11,12,30) , "" );

}

void test_extents_extraction()
{
  using data_type = int[][20][30][40] ;
  using dim_t = typename std::experimental::detail::extract_extents< data_type , int >::type ;

  static_assert( dim_t::rank() == 4 , "" );
  static_assert( dim_t::rank_dynamic() == 1 , "" );
  static_assert( dim_t::static_extent<0>() == 0 , "" );
  static_assert( dim_t::static_extent<1>() == 20 , "" );
  static_assert( dim_t::static_extent<2>() == 30 , "" );
  static_assert( dim_t::static_extent<3>() == 40 , "" );
  static_assert( dim_t::static_extent<4>() == 1 , "" );
}

int main()
{
  test_extents_constexpr();
  test_extents_extraction();
  return 0 ;
}

