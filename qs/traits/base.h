#ifndef QS_TRAITS_BASE_H
#define QS_TRAITS_BASE_H

#include <qs/config.h>
#include <qs/meta.h>

#include <type_traits>


QS_NAMESPACE_BEGIN


// -----------------------------------------------------------------------------
// [concepts.arithmetic]
// -----------------------------------------------------------------------------

template<class T>
using is_signed_integral = conjunction<std::is_integral<T>, std::is_signed<T>>;


// -----------------------------------------------------------------------------
// [concept.same]
// -----------------------------------------------------------------------------

template<class T, class U = void>
struct is_same_as : conjunction<std::is_same<T, U>, std::is_same<U, T>>
{};


// -----------------------------------------------------------------------------
// [concept.derived]
// -----------------------------------------------------------------------------

template<class Derived, class Base>
struct is_derived_from
    : conjunction<std::is_base_of<Base, Derived>, std::is_convertible<Derived const volatile*, Base const volatile*>>
{};


// -----------------------------------------------------------------------------
// [concept.convertible]
// -----------------------------------------------------------------------------

template<class From, class To>
struct is_convertible_to
{
private:
    template<class F, class T>
    static auto test_static_cast(int) -> decltype(static_cast<T>(std::declval<F>()), std::true_type{});
    template<class, class>
    static auto test_static_cast(...) -> std::false_type;

public:
    static constexpr bool value =
        std::is_convertible<From, To>::value && decltype(test_static_cast<From, To>(0))::value;
};


// -----------------------------------------------------------------------------
// [concepts.booleantestable]
// -----------------------------------------------------------------------------

template<class B>
struct is_boolean_testable
    : conjunction<is_convertible_to<B, bool>, meta::test::op_negation<B, meta::fn_back<is_convertible_to, bool>>>
{};


// -----------------------------------------------------------------------------
// [concept.destructible]
// -----------------------------------------------------------------------------

template<class T>
struct is_destructible : std::is_nothrow_destructible<T>
{};


// -----------------------------------------------------------------------------
// [concept.constructible]
// -----------------------------------------------------------------------------

template<class T, class... Args>
struct is_constructible_from : conjunction<is_destructible<T>, std::is_constructible<T, Args...>>
{};


// -----------------------------------------------------------------------------
// [concept.default.init]
// -----------------------------------------------------------------------------

template<class T>
struct is_default_initializable
    : conjunction<is_constructible_from<T>, meta::test::init_default<T>, meta::test::init_default_new<T>>
{};

// -----------------------------------------------------------------------------
// [concept.moveconstructible]
// -----------------------------------------------------------------------------

template<class T>
struct is_move_constructible : conjunction<is_constructible_from<T, T>, is_convertible_to<T, T>>
{};


// -----------------------------------------------------------------------------
// [concept.copyconstructible]
// -----------------------------------------------------------------------------

template<class T>
struct is_copy_constructible
    : conjunction<is_move_constructible<T>, is_constructible_from<T, T&>, is_convertible_to<T&, T>,
                  is_constructible_from<T, T const&>, is_convertible_to<T const&, T>, is_constructible_from<T, T const>,
                  is_convertible_to<T const, T>>
{};


// -----------------------------------------------------------------------------
// [concept.copycv]
// -----------------------------------------------------------------------------

namespace intl
{
    template<class From, class To>
    struct copy_cv : type_identity<To>
    {};
    template<class From, class To>
    struct copy_cv<From const, To> : type_identity<add_const_t<To>>
    {};
    template<class From, class To>
    struct copy_cv<From volatile, To> : type_identity<typename std::add_volatile<To>::type>
    {};
    template<class From, class To>
    struct copy_cv<From const volatile, To> : type_identity<typename std::add_cv<To>::type>
    {};

    template<class From, class To>
    using copy_cv_t = typename copy_cv<From, To>::type;


    template<class From, class To>
    struct copy_cv_ref : type_identity<copy_cv_t<From, To>>
    {};
    template<class From, class To>
    struct copy_cv_ref<From&, To> : type_identity<add_lvalue_reference_t<copy_cv_t<From, To>>>
    {};
    template<class From, class To>
    struct copy_cv_ref<From&&, To> : type_identity<add_rvalue_reference_t<copy_cv_t<From, To>>>
    {};

    template<class From, class To>
    using copy_cv_ref_t = typename copy_cv_ref<From, To>::type;

} // namespace intl


// -----------------------------------------------------------------------------
// [concept.commonref]
// -----------------------------------------------------------------------------

// class to be overridden by the user
template<typename T, typename U, template<typename> class TQual, template<typename> class UQual>
struct basic_common_reference
{};


namespace intl
{
    template<class X, class Y>
    using ternary_res_t = decltype(false ? std::declval<X (&)()>()() : std::declval<Y (&)()>()());

    template<class From>
    struct copy_cvref_curry
    {
        template<class To>
        using invoke = copy_cv_ref_t<From, To>;
    };

    template<class T, class U>
    using basic_common_reference_res_t =
        typename basic_common_reference<remove_cvref_t<T>, remove_cvref_t<U>, copy_cvref_curry<T>::template invoke,
                                        copy_cvref_curry<U>::template invoke>::type;

    // Let A and B be two types to get the common reference of. We denote X, Y as remove_reference_t<A>,
    // remove_reference_t<B> respectively.

    template<class A, class B, class X = remove_reference_t<A>, class Y = remove_reference_t<B>, class VoidT = void,
             class EnableIf = void>
    struct common_ref_impl;

    template<class X, class Y>
    using common_ref_impl_t = typename common_ref_impl<X, Y>::type;

    // If A and B are both lvalue reference types, then common_ref_impl<A, B> is the ternary resolution of
    // (copy_cv_t<X>::apply<Y> &) and (copy_cv_t<Y>::apply<X> &)

    template<class X, class Y>
    using cv_ternary_res_t = ternary_res_t<copy_cv_t<X, Y>&, copy_cv_t<Y, X>&>;

    template<class A, class B, class X, class Y>
    struct common_ref_impl<A&, B&, X, Y, void_t<cv_ternary_res_t<X, Y>>,
                           enable_if_t<std::is_reference<cv_ternary_res_t<X, Y>>::value>>
    {
        using type = cv_ternary_res_t<X, Y>;
    };

    // We denote the type common_ref_rrref_res_t<X, Y> as remove_reference_t<common_ref_impl_t<X&, Y&>> &&.
    // If A and B are both rvalue reference types, then common_ref_impl<A, B> is common_ref_rrref_res_t<X, Y> if that
    // type exists and both A, B are convertible to common_ref_rrref_res_t<X, Y>.

    template<class T, class U>
    using common_ref_rrref_res_t = remove_reference_t<common_ref_impl_t<T&, U&>>&&;

    template<class A, class B, class X, class Y>
    struct common_ref_impl<A&&, B&&, X, Y, void_t<common_ref_rrref_res_t<X, Y>>,
                           enable_if_t<std::is_convertible<A&&, common_ref_rrref_res_t<X, Y>>::value &&
                                       std::is_convertible<B&&, common_ref_rrref_res_t<X, Y>>::value>>
    {
        using type = common_ref_rrref_res_t<X, Y>;
    };

    // We denote the type common_ref_lrref_res_t<X, Y> as common_ref_impl_t<T const&, U&>.
    // If A is rvalue reference and B is lvalue reference (or vice-versa), then common_ref_impl<A, B> is
    // common_ref_lrref_res_t<X, Y> if that type exists and both A (or B) are convertible to
    // common_ref_lrref_res_t<X, Y>.

    template<class T, class U>
    using common_ref_lrref_res_t = common_ref_impl_t<T const&, U&>;

    template<class A, class B, class X, class Y>
    struct common_ref_impl<A&&, B&, X, Y, void_t<common_ref_lrref_res_t<X, Y>>,
                           enable_if_t<std::is_convertible<A&&, common_ref_lrref_res_t<X, Y>>::value>>
    {
        using type = common_ref_lrref_res_t<X, Y>;
    };

    template<class A, class B, class X, class Y>
    struct common_ref_impl<A&, B&&, X, Y> : common_ref_impl<B&&, A&>
    {};

    // common_ref_selector is a helper struct that implements the common_reference concept as described in the C++20
    // standard. The implementation is based on the following rules:
    // 1. If both types are reference types and the common_ref_impl<T, U> is well-formed, then common_reference<T, U> is
    // that type.
    // 2. If the basic_common_reference_t<remove_cvref_t<T>, remove_cvref_t<U>, TQual, UQual> is well-formed, then the
    // common_reference<T, U> is that type (TQual and UQual are transform meta-functions to get the cv-qualifiers of T
    // and U).
    // 3. If ternary_res_t<T, U> is well-formed, then the common_reference<T, U> is that type.
    // 4. Otherwise, common_reference<T, U> reduces to std::common_type<T, U>.

    template<class T, class U, class = void>
    struct common_ref_selector3 : std::common_type<T, U>
    {};
    template<class T, class U>
    struct common_ref_selector3<T, U, void_t<ternary_res_t<T, U>>>
    {
        using type = ternary_res_t<T, U>;
    };

    template<class T, class U, class = void>
    struct common_ref_selector2 : common_ref_selector3<T, U>
    {};
    template<class T, class U>
    struct common_ref_selector2<T, U, void_t<basic_common_reference_res_t<T, U>>>
    {
        using type = basic_common_reference_res_t<T, U>;
    };

    template<class T, class U, class = void, class = void>
    struct common_ref_selector : common_ref_selector2<T, U>
    {};
    template<class T, class U>
    struct common_ref_selector<T, U, void_t<common_ref_impl_t<T, U>>,
                               enable_if_t<std::is_reference<T>::value && std::is_reference<U>::value>>
    {
        using type = common_ref_impl_t<T, U>;
    };
} // namespace intl


template<class...>
struct common_reference;

template<class... Args>
using common_reference_t = typename common_reference<Args...>::type;

template<>
struct common_reference<>
{};

template<class T>
struct common_reference<T> : type_identity<T>
{};

template<class T, class U>
struct common_reference<T, U> : intl::common_ref_selector<T, U>
{};

template<class T, class U, class V, class... Rest>
struct common_reference<T, U, V, Rest...> : common_reference<common_reference_t<T, U>, V, Rest...>
{};


template<class T, class U, class = void>
struct is_common_reference_with : std::false_type
{};

template<class T, class U>
struct is_common_reference_with<T, U, void_t<common_reference_t<T, U>, common_reference_t<U, T>>>
    : conjunction<is_same_as<common_reference_t<T, U>, common_reference_t<U, T>>,
                  is_convertible_to<T, common_reference_t<T, U>>, is_convertible_to<U, common_reference_t<T, U>>>
{};


// -----------------------------------------------------------------------------
// [concept.assignable]
// -----------------------------------------------------------------------------

template<class Lhs, class Rhs>
struct is_assignable_from
    : conjunction<std::is_lvalue_reference<Lhs>, is_common_reference_with<make_cref_t<Lhs>, make_cref_t<Lhs>>,
                  meta::test::op_assign<Lhs, Rhs&&, meta::fn<is_same_as, Lhs>>>
{};


// -----------------------------------------------------------------------------
// [concept.swappable]
// -----------------------------------------------------------------------------

template<class T>
struct is_swappable : std::is_swappable<T>
{};


template<class T, class U>
struct is_swappable_with : std::is_swappable_with<T, U>
{};


// -----------------------------------------------------------------------------
//  [concepts.object]
// -----------------------------------------------------------------------------

template<class T>
struct is_movable : conjunction<std::is_object<T>, is_move_constructible<T>, is_assignable_from<T&, T>, is_swappable<T>>
{};


template<class T>
struct is_copyable : conjunction<is_copy_constructible<T>, is_movable<T>, is_assignable_from<T&, T&>,
                                 is_assignable_from<T&, T const&>, is_assignable_from<T&, T const>>
{};


// -----------------------------------------------------------------------------
//  [concept.equalitycomparable]
// -----------------------------------------------------------------------------

template<class T, class U>
struct is_weakly_equality_comparable_with
    : conjunction<meta::test::op_equal_to<make_cref_t<T>, make_cref_t<U>, meta::fn<is_boolean_testable>>,
                  meta::test::op_equal_to<make_cref_t<U>, make_cref_t<T>, meta::fn<is_boolean_testable>>,
                  meta::test::op_unequal_to<make_cref_t<T>, make_cref_t<U>, meta::fn<is_boolean_testable>>,
                  meta::test::op_unequal_to<make_cref_t<U>, make_cref_t<T>, meta::fn<is_boolean_testable>>>
{};


template<class T>
struct is_equality_comparable : is_weakly_equality_comparable_with<T, T>
{};


template<class T, class U, class = void>
struct is_equality_comparable_with : std::false_type
{};

template<class T, class U>
struct is_equality_comparable_with<T, U, void_t<common_reference_t<make_cref_t<T>, make_cref_t<U>>>>
    : conjunction<is_common_reference_with<make_cref_t<T>, make_cref_t<U>>,
                  is_equality_comparable<common_reference_t<make_cref_t<T>, make_cref_t<U>>>,
                  is_weakly_equality_comparable_with<T, U>>
{};


// -----------------------------------------------------------------------------
//  [concept.totallyordered]
// -----------------------------------------------------------------------------

template<class T, class U>
struct is_partially_ordered_with
    : conjunction<meta::test::op_lesser<make_cref_t<T>, make_cref_t<U>, meta::fn<is_boolean_testable>>,
                  meta::test::op_greater<make_cref_t<T>, make_cref_t<U>, meta::fn<is_boolean_testable>>,
                  meta::test::op_lesser_equal<make_cref_t<T>, make_cref_t<U>, meta::fn<is_boolean_testable>>,
                  meta::test::op_greater_equal<make_cref_t<T>, make_cref_t<U>, meta::fn<is_boolean_testable>>,
                  // reversed order of T and U
                  meta::test::op_lesser<make_cref_t<U>, make_cref_t<T>, meta::fn<is_boolean_testable>>,
                  meta::test::op_greater<make_cref_t<U>, make_cref_t<T>, meta::fn<is_boolean_testable>>,
                  meta::test::op_lesser_equal<make_cref_t<U>, make_cref_t<T>, meta::fn<is_boolean_testable>>,
                  meta::test::op_greater_equal<make_cref_t<U>, make_cref_t<T>, meta::fn<is_boolean_testable>>>
{};


template<class T>
struct is_totally_ordered : conjunction<is_equality_comparable<T>, is_partially_ordered_with<T, T>>
{};


template<class T, class U>
struct is_totally_ordered_with
    : conjunction<is_totally_ordered<T>, is_totally_ordered<U>, is_equality_comparable_with<T, U>,
                  is_totally_ordered<common_reference_t<make_cref_t<T>, make_cref_t<U>>>,
                  is_partially_ordered_with<T, U>>
{};


QS_NAMESPACE_END

#endif // CONCEPTS_H
