#ifndef QS_CONCEPTS_H
#define QS_CONCEPTS_H

#include <iterator>
#include <memory>
#include <qs/config.h>
#include <set>
#include <type_traits>
#include <utility>

// need a lot of work
// aka write all iterator concepts from scratch to be cpp17 compatible 

QS_NAMESPACE_BEGIN

template<class Range>
using iterator_t = decltype(std::begin(std::declval<Range&>()));

template<class Range>
using sentinel_t = decltype(std::end(std::declval<Range&>()));

template<class DereferenceableIterator>
using iter_reference_t = decltype(*std::declval<DereferenceableIterator&>());

template<class Iterator>
using iter_value_t = typename std::iterator_traits<remove_cvref_t<Iterator>>::value_type;

template<class Iterator>
using iter_difference_t = typename std::iterator_traits<remove_cvref_t<Iterator>>::difference_type;

template<class Iterator>
using iter_category_t = typename std::iterator_traits<remove_cvref_t<Iterator>>::iterator_category;

// clang-format off
template<class Iterator, class IteratorTag, class Element = void>
struct is_std_iterator : std::integral_constant<bool,
    is_std_iterator<Iterator, IteratorTag, void>::value &&
    std::is_constructible<Element, iter_reference_t<Iterator>>::value> {};

template<class Iterator, class IteratorTag>
struct is_std_iterator<Iterator, IteratorTag, void> : 
    std::is_convertible<iter_category_t<Iterator>, IteratorTag> {};

template<class Iterator, class IteratorTag, class Element = void>
struct is_std_iterator_exact : 
    std::integral_constant<bool, 
        is_std_iterator_exact<Iterator, IteratorTag, void>::value && 
        std::is_constructible<Element, iter_reference_t<Iterator>>::value> {};

template<class Iterator>
struct is_std_iterator_exact<Iterator, std::input_iterator_tag, void> : 
    std::integral_constant<bool,
        std::is_convertible<iter_category_t<Iterator>, std::input_iterator_tag>::value &&
        !std::is_convertible<iter_category_t<Iterator>, std::forward_iterator_tag>::value> {};

template<class Iterator>
struct is_std_iterator_exact<Iterator, std::forward_iterator_tag, void> : 
    std::integral_constant<bool,
        std::is_convertible<iter_category_t<Iterator>, std::forward_iterator_tag>::value &&
        !std::is_convertible<iter_category_t<Iterator>, std::bidirectional_iterator_tag>::value> {};

template<class Iterator>
struct is_std_iterator_exact<Iterator, std::bidirectional_iterator_tag, void> : 
    std::integral_constant<bool,
        std::is_convertible<iter_category_t<Iterator>, std::bidirectional_iterator_tag>::value &&
        !std::is_convertible<iter_category_t<Iterator>, std::random_access_iterator_tag>::value> {};

template<class Iterator>
struct is_std_iterator_exact<Iterator, std::random_access_iterator_tag, void> : 
    std::is_convertible<iter_category_t<Iterator>, std::random_access_iterator_tag> {};

// clang-format on

#if defined(__cpp_deduction_guides)
template<class Iterator, class IteratorTag, class Element = void>
inline constexpr bool is_std_iterator_v = is_std_iterator<Iterator, IteratorTag, Element>::value;
template<class Iterator, class IteratorTag, class Element = void>
inline constexpr bool is_std_iterator_exact_v = is_std_iterator_exact<Iterator, IteratorTag, Element>::value;
#endif // __cpp_deduction_guides

// to_address

namespace intl
{
    template<class Pointer, class = void>
    struct has_pointer_traits_to_address : std::false_type
    {};

    template<class Pointer>
    struct has_pointer_traits_to_address<Pointer, decltype(std::pointer_traits<Pointer>::to_address(
                                                      std::declval<Pointer const&>()))> : std::true_type
    {};

    template<class Pointer, class = void>
    struct has_operator_arrow : std::false_type
    {};

    template<class Pointer>
    struct has_operator_arrow<Pointer, decltype((void)std::declval<Pointer const&>().operator->())> : std::true_type
    {};

    template<class Pointer>
    struct is_non_trivial_pointer
    {
        static constexpr bool value =
            has_operator_arrow<Pointer>::value || has_pointer_traits_to_address<Pointer>::value;
    };

    template<class Pointer, class = void>
    struct to_address_helper;

    template<class T>
    QS_CONSTEXPR11 T* to_address(T* p) noexcept
    {
        static_assert(!std::is_function<T>::value, "T is a function type");
        return p;
    }

    template<class Pointer,
             class = enable_if_t<std::is_class<Pointer>::value && is_non_trivial_pointer<Pointer>::value>>
    QS_CONSTEXPR11 decay_t<decltype(to_address_helper<Pointer>::call(std::declval<Pointer const&>()))>
                   to_address(Pointer const& p) noexcept
    {
        return to_address_helper<Pointer>::call(p);
    }

    template<class Pointer, class>
    struct to_address_helper
    {
        QS_CONSTEXPR11 static decltype(to_address(std::declval<Pointer const&>().operator->()))
        call(Pointer const& p) noexcept
        {
            return to_address(p.operator->());
        }
    };

    template<class Pointer>
    struct to_address_helper<Pointer,
                             decltype(std::pointer_traits<Pointer>::to_address(std::declval<Pointer const&>()))>
    {
        QS_CONSTEXPR11 static decltype(std::pointer_traits<Pointer>::to_address(std::declval<Pointer const&>()))
        call(Pointer const& p) noexcept
        {
            return std::pointer_traits<Pointer>::to_address(p);
        }
    };

    // Primary template: Defaults to `false`
    template<class Ptr, class T, bool IsRawPtr = std::is_same<Ptr, T*>::value, class = void>
    struct is_pointer_like : std::false_type
    {};

    // Specialization: Check if `Ptr` is a **raw pointer (`T*`)** and matches `ValueType`
    template<class Ptr, class T, class Enable>
    struct is_pointer_like<Ptr, T, true, Enable> : std::true_type
    {};

    // Specialization: Check if `Ptr` has `operator->()` that returns a type convertible to `ValueType*`
    template<class Ptr, class T>
    struct is_pointer_like<
        Ptr, T, false,
        typename std::enable_if<std::is_convertible<decltype(std::declval<Ptr>().operator->()), T*>::value>::type>
        : std::true_type
    {};

    // Specialization: Check if `Ptr` has `std::iterator_traits<Ptr>::pointer` convertible to `ValueType*`
    template<class Ptr, class T>
    struct is_pointer_like<
        Ptr, T, false,
        typename std::enable_if<std::is_convertible<
            decltype(std::iterator_traits<Ptr>::to_address(std::declval<Ptr const&>())), T*>::value>::type>
        : std::true_type
    {};

} // namespace intl


// template<class Range>
// class is_range
// {
//     template<class V>
//     static auto test(int) -> decltype(std::begin(std::declval<V&>()), std::end(std::declval<V&>()),
//     std::true_type{}); template<class V> static auto test(...) -> std::false_type;

// public:
//     static constexpr bool value = decltype(test<Range>(0))::value;
// };

// template<class Range>
// class is_sized_range
// {
//     template<class V>
//     static auto test(int) -> decltype(std::declval<V&>().size(), std::true_type{});
//     template<class V>
//     static auto test(...) -> std::false_type;

// public:
//     static constexpr bool value = is_range<Range>::value && decltype(test<Range>(0))::value;
// };

// template<class Range>
// struct is_contiguous_range
// {
//     template<class V>
//     static auto test(int) -> decltype(std::data(std::declval<V&>()), std::true_type{});
//     template<class V>
//     static auto test(...) -> std::false_type;

// public:
//     static constexpr bool value =
//         is_sized_range<Range>::value && is_std_iterator<iterator_t<Range>, std::random_access_iterator_tag> &&
//         is_contiguous_iterator<iterator_t<Range>> &&
//         requires(_Tp& __t) { ranges::data(__t) } -> same_as<add_pointer_t<range_reference_t<_Tp>>>;
//     decltype(test<Range>(0))::value;
// };


QS_NAMESPACE_END

#endif // CONCEPTS_H
