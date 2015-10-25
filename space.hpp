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

template <std::size_t... Exts>
struct extents;

namespace detail
{

// Builds a std::tuple with one entry for each dynamic extent.
template <typename Tuple, std::size_t... Exts>
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
// extents as Exts when calling. 
template <std::size_t Idx, std::size_t MappedIdx, std::size_t... Exts>
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

// Counts the number of dynamic extents.
template <std::size_t... Exts>
struct count_dynamic_extents;

// Base case.
template <>
struct count_dynamic_extents<> : std::integral_constant<std::size_t, 0> {};

template <std::size_t Head, std::size_t... Tail>
struct count_dynamic_extents<Head, Tail...>
  : std::integral_constant<std::size_t,
        ( Head == dynamic_extent
        ? count_dynamic_extents<Tail...>::value + 1
        : count_dynamic_extents<Tail...>::value) 
    >{};

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

}} // boost::detail

namespace std
{

template <std::size_t... Exts>
struct rank<boost::extents<Exts...> >
  : std::integral_constant<std::size_t, sizeof...(Exts)> {};

template <std::size_t Head, std::size_t... Tail>
struct extent<boost::extents<Head, Tail...>, 0>
  : std::integral_constant<std::size_t, Head> {};

template <std::size_t Head, std::size_t... Tail, unsigned ND>
struct extent<boost::extents<Head, Tail...>, ND>
  : std::integral_constant<
        std::size_t
      , std::extent<boost::extents<Tail...>, ND - 1>::value
    > {};

} // std

namespace boost
{

namespace detail
{

template <std::size_t I, std::size_t... Exts>
constexpr typename extents<Exts...>::size_type
get_value_impl(extents<Exts...>, std::true_type) noexcept;

} // detail

template <std::size_t... Exts>
struct extents
{
    // TYPES

    using size_type = std::size_t;

    // NOTE: Not defined in the spec, public for unit tests. 
    using dynamic_extents_type =
        typename detail::build_extents_tuple<std::tuple<>, Exts...>::type;

    // CONSTRUCTORS, DESTRUCTORS, ASSIGNMENT OPERATORS

    // Constructs from a set of dynamic extents.
    // EXPECTS: num_dynamic() == sizeof...(DynamicExts)
    // EXPECTS: std::is_integral<> is true for all the types in Sizes
    template <typename... DynamicExts>
    constexpr extents(DynamicExts... exts) noexcept
      : dynamic_extents(exts...)
    {
        static_assert(
            detail::pack_is_integral<DynamicExts...>::value
          , "Non-integral types passed to extents<> constructor" 
            );
        static_assert(
            detail::count_dynamic_extents<Exts...>::value
            == sizeof...(DynamicExts)
          , "Incorrect number of dynamic extents passed to extents<>."
            );
    }

    // Copy constructor.
    constexpr extents(extents const&) = default;

    // Move constructor.
    constexpr extents(extents&&) = default;

    // Copy assignment operator.
    extents& operator=(extents const&) = default;

    // Move assignment operator.
    extents& operator=(extents&&) = default;

    // METADATA ACCESS

    // Returns the number of dimensions of the referenced array.
    static constexpr size_type rank() noexcept
    {
        return std::rank<extents>::value;
    }

    // Returns the number of extents which are dynamic.
    static constexpr size_type num_dynamic() noexcept
    {
        return detail::count_dynamic_extents<Exts...>::value;
    }

  private:
    template <std::size_t I, std::size_t... BExts>
    friend constexpr typename extents<BExts...>::size_type
    detail::get_value_impl(extents<BExts...>, std::true_type) noexcept;

    dynamic_extents_type dynamic_extents;
};

namespace detail
{

// Dynamic extent.
template <std::size_t I, std::size_t... Exts>
constexpr typename extents<Exts...>::size_type
get_value_impl(extents<Exts...> ext, std::true_type) noexcept
{
    typedef dynamic_extent_tuple_index<I, 0, Exts...> mapping;
    return std::get<mapping::type::value>(ext.dynamic_extents);
}

// Static extent.
template <std::size_t I, std::size_t... Exts>
constexpr typename extents<Exts...>::size_type
get_value_impl(extents<Exts...> ext, std::false_type) noexcept
{
    return std::extent<extents<Exts...>, I>::value;
}

} // detail

// Returns the number of elements in the Ith direction.
template <std::size_t I, std::size_t... Exts>
constexpr typename std::enable_if<
    I < extents<Exts...>::rank() 
  , typename extents<Exts...>::size_type
>::type get_value(extents<Exts...> ext) noexcept
{
    typedef std::integral_constant<
        bool, std::extent<extents<Exts...>, I>::value == dynamic_extent
    > dispatch;
    return detail::get_value_impl<I>(ext, dispatch());
}

namespace detail
{

template <typename ValueType, typename Exts, typename ArrayRefTraits>
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
    // EXPECTS: ptr != nullptr 
    // EXPECTS: std::is_integral<> is true for all the types in Sizes
    // ENSURES: this->data() == ptr
    template <typename... Sizes>
    constexpr view_impl(pointer ptr, Sizes... sizes) noexcept;

    // Conversion constructor.
    // TYPE-REQUIREMENTS: BValueType must be assignment-compatible
    template <typename OtherValueType>
    constexpr view_impl(
        view_impl<OtherValueType, Exts, ArrayRefTraits> const& b
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

    // Returns the number of dimensions of the referenced array.
    static constexpr size_type rank() noexcept;

    // ELEMENT ACCESS

    template <typename... Idx>
    typename std::enable_if<
        rank() == sizeof...(Idx) && detail::pack_is_integral<Idx...>::value
      , reference
    >::type operator()(Idx... idx) const;
};

} // detail

} // boost

#endif // BOOST_VIEW_CF36957A_A9C6_44BF_ABF1_7CD6BDEC6CF0

