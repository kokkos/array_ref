
#include <array_ref>
#include <iostream>

template< typename Dimension , unsigned N >
struct prepend_dim ;

template< typename ValueType , ValueType ... Dims  , unsigned N >
struct prepend_dim
  < std::experimental::array_property::dimension_typed<ValueType, Dims... >
  , N >
{
  using type =
   std::experimental::array_property::dimension_typed<ValueType, N , Dims... > ;
};

constexpr size_t generate_dimension_value( unsigned r )
  { return static_cast<size_t>(2) * ( r + 1 ); }

template< typename ValueType , unsigned RankStatic , unsigned RankDynamic , unsigned R = 0 >
struct generate_dimension {

  using type = typename std::conditional
    < ( RankStatic < RankDynamic )
    , typename prepend_dim
         < typename generate_dimension<ValueType,RankStatic,(RankDynamic?RankDynamic-1:0),R+1>::type , 0 >::type
    , typename prepend_dim
         < typename generate_dimension<ValueType,(RankStatic?RankStatic-1:0),RankDynamic,R+1>::type , generate_dimension_value(R) > ::type
    >::type ;
};

template< typename ValueType , unsigned R >
struct generate_dimension< ValueType , 0 , 0 , R >
{
  using type = std::experimental::array_property::dimension_typed<ValueType> ;
};

template< typename ValueType , unsigned RankStatic , unsigned R >
struct generate_dimension< ValueType , RankStatic , 0 , R >
{
  using nest = typename generate_dimension<ValueType,RankStatic-1,0,R+1>::type ;
  using type = typename prepend_dim< nest , generate_dimension_value(R) >::type ;
};

template< typename ValueType , unsigned RankDynamic , unsigned R >
struct generate_dimension< ValueType , 0 , RankDynamic , R >
{
  using nest = typename generate_dimension<ValueType,0,RankDynamic-1,R+1>::type ;
  using type = typename prepend_dim< nest , 0 >::type ;
};

void test_dimension_constexpr()
{
  using dim_size_t_00_00_t = typename generate_dimension<size_t,0,0>::type ;
  using dim_int_00_00_t    = typename generate_dimension<int,0,0>::type ;
  using dim_int_01_00_t    = typename generate_dimension<int,1,0>::type ;
  using dim_int_00_01_t    = typename generate_dimension<int,0,1>::type ;
  using dim_int_01_01_t    = typename generate_dimension<int,1,1>::type ;
  using dim_int_07_00_t    = typename generate_dimension<int,7,0>::type ;
  using dim_int_00_07_t    = typename generate_dimension<int,0,7>::type ;
  using dim_int_07_07_t    = typename generate_dimension<int,7,7>::type ;

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

  static_assert( dim_int_07_07_value.in_bounds(1,2,3,4,5,6,7,8,9,10,11,12,13) , "" );
  static_assert( ! dim_int_07_07_value.in_bounds(1,2,3,4,5,6,7,8,9,10,11,12,30) , "" );

}

int main()
{
  test_dimension_constexpr();
  return 0 ;
}

