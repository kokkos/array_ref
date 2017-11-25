
#include <iostream>
#include "../demangle.h"
#include <type_traits>

template <typename> struct mdspan {  };

using diabolic_t = int[][3][5];
using more_diabolic_t = int[3][][5];
using complete_array_t = int[3][5][7];

using diabolic_build_up_t = diabolic_t[];
using diabolic_build_up_okay_t = complete_array_t[];

//------------------------------------------------------------------------------

// This works by the decaying the argument to int(*)[]
void f(int v[][]) {
  using T = int[];
  static_assert(std::is_same_v<T*, decltype(v)>);
}

//------------------------------------------------------------------------------

// This should fail (and does)
//more_diabolic_t f() {  }

// This shouldn't fail (and doesn't)
mdspan<more_diabolic_t> f() {  }

//------------------------------------------------------------------------------

// This shouldn't fail (and doesn't)
void f(mdspan<more_diabolic_t>) {  }

// This shouldn't fail (and doesn't)
void f(mdspan<more_diabolic_t[][10][][9][]>) {  }

//------------------------------------------------------------------------------

// This does work on current compilers
diabolic_t& test() { }

// Q: should this work, given behavior above on current compilers?
// (it currently does with the patch I made)
more_diabolic_t& test_more() { }

// Fails with gcc, passes with clang (!!?!)
void test2(diabolic_t&);

// *BUT* both compilers accept this even if T is deduced to be diabolic_t
template <typename T>
void test2_deduced(T&) {  }
// (same thing with pointers to incomplete types)

// Q: Should this fail??!?? (doesn't with current patch)
void test2(more_diabolic_t&) { }

//------------------------------------------------------------------------------

// This shouldn't fail (and doesn't)
void test3(diabolic_build_up_okay_t) {  }

// This shouldn't fail.  It should decay the type of v to diabolic_build_up_okay_t*
void test3(diabolic_build_up_okay_t v[]) {
  static_assert(std::is_same_v<diabolic_build_up_okay_t*, decltype(v)>);
}

// This should fail (and does)
//void test3(diabolic_build_up_t) {  }

// This shouldn't fail. It should decay the type of v to int(*)[5][][3]
void test_foo(int v[][5][][3]) {
  using T = int[5][][3];
  static_assert(std::is_same_v<T*, decltype(v)>);
}

// This should fail (and it doesn't)
void test_foo_2(int v[5][3][][5]) {
  // This shouldn't fail. It should decay the type of v to int(*)[3][][5]
  using T = int[3][][5];
  static_assert(std::is_same_v<T*, decltype(v)>);

}

//------------------------------------------------------------------------------

template <typename T>
struct sfinae_test_helper {
  using type = T[5][];
};

// This should fail for all T since it returns an array type (not
// just because it's an incomplete array type)
template <typename T>
typename sfinae_test_helper<T>::type
sfinae_test(T) {  }

double sfinae_test(...) {  }

//------------------------------------------------------------------------------

//template <typename T>
//int sfinae_test_2(T) {  }
//
//template <typename T>
//double sfinae_test_2(T(&)[][]) {  }
//
//float sfinae_test_2(...) {  }

//------------------------------------------------------------------------------

struct TestClass {

  // This shouldn't fail (and doesn't). The type of v should be decay to int(*)[]
  void foo(int v[][]) {
    using T = int[];
    static_assert(std::is_same_v<T*, decltype(v)>);
  }

  // This shouldn't fail (and doesn't). The type of v should be decay to int(*)[3][][5]
  void foo(int v[][3][][5]) {
    using T = int[3][][5];
    static_assert(std::is_same_v<T*, decltype(v)>);
  }

};

//------------------------------------------------------------------------------

template <typename T>
struct TestTemplate {

  void foobar(T) {  }

};

// This should fail (and does)
//auto result_of_test() {
//  return *(int(*)[][])nullptr;
//}

//------------------------------------------------------------------------------

template <typename T>
auto result_of_test_2() {
  return *(T*)nullptr;
}

//------------------------------------------------------------------------------

int main()
{
  // This is currently allowed:
  diabolic_t* test = nullptr;

  // Q: Should this fail? (Probably not)
  more_diabolic_t* test_more = nullptr;

  // This should fail (and does)
  //diabolic_build_up_t test2 = nullptr;

  // This should fail (and does)
  //int x[][] = { {1, 2, 3}, {4, 5, 6} };

  // This shouldn't fail (and doesn't);
  TestTemplate<int[][]> t;
  // This does fail, though it's because of the dereference rather than the template
  //t.foobar(*(int(*)[][])&t);

  // This should probably be a substitution failure (so this static assert
  // should probably work)
  static_assert(std::is_same_v<
    decltype(sfinae_test(*(int(*)[])nullptr)), double
  >);

  //static_assert(std::is_same_v<
  //  decltype(sfinae_test_2((int(&)[][])t)), double
  //>);

  // This should fail (and it does)
  //using bad_result_t = std::result_of_t<decltype(&result_of_test_2<int[][]>)>;

  // This shouldn't work in gcc (since the non-templated version doesn't work)
  test2_deduced(*(diabolic_t*)nullptr);

  // This shouldn't work (and doesn't) because of the dereference
  //test2_deduced(*(more_diabolic_t*)nullptr);


  static_assert(std::rank<int[5][][6][][7]>::value == 5);
  static_assert(std::extent<int[5][][6][][7]>::value == 5);
  static_assert(std::extent<int[5][][6][][7], 3>::value == 0);

  static_assert(std::is_same<std::remove_extent_t<int[5][][6][][7]>, int[][6][][7]>::value);
  static_assert(std::is_same<std::remove_extent_t<int[][5][6][][7]>, int[5][6][][7]>::value);
  static_assert(std::is_same<std::remove_extent_t<int[][][6][][7]>, int[][6][][7]>::value);

  static_assert(std::is_same<std::remove_all_extents_t<int[5][][6][][7]>, int>::value);

  static_assert(std::is_same<std::decay_t<int[5][][6][][7]>, int(*)[][6][][7]>::value);
  //static_assert(std::is_same<std::remove_pointer_t<std::decay_t<int[5][][6][][7]>>, int[6][][7]>::value);
  // causes crash in compiler
  //std::cout << demangled_name<std::remove_pointer_t<std::decay_t<int[5][][6][][7]>>>() << std::endl;

}

