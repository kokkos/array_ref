////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2015 Bryce Adelstein Lelbach aka wash
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if !defined(BOOST_VIEW_CF36957A_A9C6_44BF_ABF1_7CD6BDEC6CF0)
#define BOOST_VIEW_CF36957A_A9C6_44BF_ABF1_7CD6BDEC6CF0

#include <cstdint>

#include <tuple>
#include <type_traits>

#include <boost/mpl/and.hpp>

namespace boost
{

constexpr std::size_t dynamic_extent = std::size_t(-1);

namespace detail
{

// Builds a std::tuple with one entry for each dynamic extent.
template <typename Tuple, std::size_t... Dimensions>
struct build_extents_tuple;

// Base case.
template <typename... T>
struct build_extents_tuple<std::tuple<T...> >
{
    using type = std::tuple<T...>;
};

template <typename... T, std::size_t Head, std::size_t... Tail>
struct build_extents_tuple<std::tuple<T...>, Head, Tail...>
  : build_extents_tuple<
        typename std::conditional<
            Head == dynamic_extent
          , std::tuple<std::size_t, T...> 
          , std::tuple<T...> 
        >::type
      , Tail...
    > {};

// Maps an actual dynamic extent index (Idx) to an index in the dynamic extent
// tuple. E.g. if you have extents<3, dynamic_extent, 4, dynamic_extent>, this
// metafunction would map 1 to 0 (the first dynamic extent) and 3 to 1 (the
// second one). Pass the input index as Idx, 0 for MappedIdx and the list of
// extents as Dimensions when calling. 
template <std::size_t Idx, std::size_t MappedIdx, std::size_t... Dimensions>
struct dynamic_extent_tuple_index;

// Base case.
template <std::size_t Idx, std::size_t MappedIdx>
struct dynamic_extent_tuple_index<Idx, MappedIdx>
  : std::integral_constant<std::size_t, MappedIdx> {};

template <std::size_t Idx, std::size_t MappedIdx
        , std::size_t Head, std::size_t... Tail>
struct dynamic_extent_tuple_index<Idx, MappedIdx, Head, Tail...>
  : dynamic_extent_tuple_index<
        (Idx != 0 ? Idx - 1 : Idx)
      , std::conditional<
            Head == dynamic_extent && Idx != 0 
          , std::integral_constant<std::size_t, MappedIdx + 1> 
          , std::integral_constant<std::size_t, MappedIdx> 
        >::type::value
      , Tail...
    > {};

}

template <std::size_t... Dimensions>
struct extents;

}

namespace std {

template <std::size_t... Dimensions>
struct rank<boost::extents<Dimensions...> >
  : std::integral_constant<std::size_t, sizeof...(Dimensions)> {};

template <std::size_t Head, std::size_t... Tail>
struct extent<boost::extents<Head, Tail...>, 0>
  : std::integral_constant<std::size_t, Head> {};

template <std::size_t Head, std::size_t... Tail, unsigned ND>
struct extent<boost::extents<Head, Tail...>, ND>
  : std::integral_constant<
        std::size_t
      , std::extent<boost::extents<Tail...>, ND - 1>::value
    > {};

}

namespace boost
{

template <std::size_t... Dimensions>
struct extents
{
    using size_type = std::size_t;

    using dynamic_extents_type =
        typename detail::build_extents_tuple<std::tuple<>, Dimensions...>::type;

    template <std::size_t Idx>
    size_type get() const noexcept
    {
        typedef std::integral_constant<
            bool, std::extent<extents, Idx>::value == dynamic_extent
        > dispatch;
        return get_impl<Idx>(dispatch());
    }

  private:
    // Dynamic extent.
    template <std::size_t Idx>
    size_type get_impl(std::true_type) const noexcept;

    // Static extent.
    template <std::size_t Idx>
    constexpr size_type get_impl(std::false_type) noexcept;

    dynamic_extents_type dynamic_extents;
};

namespace detail
{

// Metafunction which returns true if std::is_integral<> is true for all of the
// types in the parameter pack.
template <typename... T>
struct pack_is_integral;

// Base case.
template <>
struct pack_is_integral<> : std::true_type {};

template <typename Head, typename... Tail>
struct pack_is_integral<Head, Tail...>
  : boost::mpl::and_<std::is_integral<Head>, pack_is_integral<Tail...> > {};

template <typename ValueType, typename Dimensions, typename Layout
        , typename... Properties>
struct view_impl
{
    // TYPES

    using size_type = std::size_t;
    using index_type = std::ptrdiff_t;

    using value_type = ValueType;
    using pointer = std::add_pointer<value_type>;
    using reference = std::add_lvalue_reference<value_type>;

    // CONSTRUCTORS, DESTRUCTORS, ASSIGNMENT OPERATORS

    // Default constructor, yields the same result as from std::nullptr_t.
    // ENSURES: this->data() == nullptr && this->size() == 0
    constexpr view_impl() noexcept;

    // Construct from nullptr, results in a zero-length view.
    constexpr view_impl(std::nullptr_t) noexcept;

    // Constructs from a pointer + list of sizes.
    // EXPECTS: all sizes >= 0
    // EXPECTS: ptr != nullptr || all sizes != 0
    // EXPECTS: Sizes are integral types
    // ENSURES: this->data() == ptr && this->size() == sum of sizes
    template <typename... Sizes>
    constexpr view_impl(pointer ptr, Sizes... sizes) noexcept;

    // Conversion constructor.
    // TYPE-REQUIREMENTS: BValueType must be assignment-compatible
    // TYPE-REQUIREMENTS: Layout and BLayout must be the same type 
    // TYPE-REQUIREMENTS: Dimensions and BDimensions must be the same type 
    // TYPE-REQUIREMENTS: Properties and BProperties must be the same type(s)
    template <typename BValueType, typename BDimensions, typename BLayout
            , typename... BProperties>
    constexpr view_impl(
        view_impl<BValueType, BDimensions, BLayout, BProperties...> const& b
        ) noexcept; 

    // FIXME: Static array constructor.

    // Copy constructor.
    constexpr view_impl(view_impl const& b) = default;

    // Move constructor.
    constexpr view_impl(view_impl&& b) = default;

    // Copy assignment operator.
    view_impl& operator=(view_impl const& b) = default;

    // Move assignment operator.
    view_impl& operator=(view_impl&& b) = default;

    // Destructor.
    ~view_impl() = default;

    // METADATA ACCESS
    static constexpr size_type rank() noexcept;

    // ELEMENT ACCESS
    template <typename... Idx>
    typename std::enable_if<
        rank() == sizeof...(Idx) && pack_is_integral<Idx...>::value
      , reference
    >::type operator()(Idx... idx) const;
};

}

}

#endif // BOOST_VIEW_CF36957A_A9C6_44BF_ABF1_7CD6BDEC6CF0

