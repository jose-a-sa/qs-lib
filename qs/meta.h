#ifndef QS_META_H
#define QS_META_H

#include <qs/config.h>

#include <type_traits>
#include <utility>

QS_NAMESPACE_BEGIN


namespace meta
{
    template<class T>
    using t = typename T::type;

    template<class T>
    constexpr auto v() noexcept
    {
        return T::value;
    }

    // meta list

    template<class... Ts>
    struct list
    {
        template<template<class...> class Fn>
        using apply = Fn<Ts...>;

        template<class... Us>
        using append = list<Ts..., Us...>;

        template<class... Us>
        using prepend = list<Us..., Ts...>;
    };

    // list concatenation

    template<class...>
    struct list_concat;

    template<class... Ls>
    using list_concat_t = t<list_concat<Ls...>>;

    template<>
    struct list_concat<> : id<list<>>
    {};

    template<class... Ts>
    struct list_concat<list<Ts...>> : id<list<Ts...>>
    {};

    template<class... Ts, class... Us>
    struct list_concat<list<Ts...>, list<Us...>> : id<list<Ts..., Us...>>
    {};

    template<class... Ts, class... Us, class... Vs, class... Rest>
    struct list_concat<list<Ts...>, list<Us...>, list<Vs...>, Rest...>
        : list_concat<list_concat_t<list<Ts...>, list<Us...>>, list<Vs...>, Rest...>
    {};

    // list reverse

    template<class>
    struct reverse_list;

    template<class L>
    using reverse_list_t = t<reverse_list<L>>;

    template<>
    struct reverse_list<list<>> : type_identity<list<>>
    {};

    template<class First>
    struct reverse_list<list<First>> : type_identity<list<First>>
    {};

    template<class First, class... Rest>
    struct reverse_list<list<First, Rest...>>
        : type_identity<typename reverse_list_t<list<Rest...>>::template prepend<First>>
    {};

    template<class... Ts>
    using reverse = reverse_list_t<list<Ts...>>;


    // apply a template to variadic arguments

    template<template<class...> class Fn, class... Args>
    using apply = Fn<Args...>;

    template<template<class...> class Fn, class... Args>
    using apply_t = meta::t<meta::apply<Fn, Args...>>;

    template<template<class...> class Fn, class... Args>
    constexpr auto apply_v()
    {
        return meta::v<meta::apply<Fn, Args...>>();
    };

    // invoke a meta-function with variadic arguments

    template<class Fn, class... Args>
    using invoke = typename Fn::template invoke<Args...>;

    template<class Fn, class... Args>
    using invoke_t = meta::t<meta::invoke<Fn, Args...>>;

    template<class Fn, class... Args>
    constexpr auto invoke_v()
    {
        return meta::v<meta::invoke<Fn, Args...>>();
    };

    // tranform a template to a invocable meta-function (with curried arguments binded in front/back)

    template<template<class...> class Fn, class... Args>
    struct fn_front
    {
        template<class... Ts>
        using invoke = Fn<Ts..., Args...>;
    };

    template<template<class...> class Fn, class... Args>
    struct fn_back
    {
        template<class... Ts>
        using invoke = Fn<Args..., Ts...>;
    };

    // default meta_functions

    template<template<class...> class Fn, class... Args>
    struct fn : fn_front<Fn, Args...>
    {};

    template<template<class...> class Fn, class... Args>
    struct fn_r
    {
        template<class... Ts>
        using invoke = typename reverse<Args..., Ts...>::template apply<Fn>;
    };

    // constant meta-function

    template<class T>
    struct constant_fn : std::true_type
    {
        template<class... Ts>
        using invoke = T;
    };

    // curry a meta-function

    template<class Fn, class... Args>
    struct curry
    {
        template<class... Ts>
        using invoke = meta::invoke<Fn, Args..., Ts...>;
    };

    // ---------------------------------------------------------------------------------------------
    // tests to check if a type has given operations or properties
    // ---------------------------------------------------------------------------------------------

    namespace test
    {
        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class init_default
        {
            template<class U, class Res = decltype(U{})>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };

        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class init_default_new
        {
            template<class U, class Res = decltype(::new U)>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };

        // -----------------------------------------------------------------------------
        // Access operators
        // -----------------------------------------------------------------------------

        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_subscript
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>()[std::declval<Rhs>()])>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class op_star
        {
            template<class U, class Res = decltype(std::declval<U>().operator*())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class U>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };


        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class op_arrow
        {
            template<class U, class Res = decltype(std::declval<U>().operator->())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class U>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };


        // -----------------------------------------------------------------------------
        // Increment and decrement operators
        // -----------------------------------------------------------------------------

        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class op_preincrement
        {
            template<class U, class Res = decltype(++std::declval<U>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class U>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };


        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class op_postincrement
        {
            template<class U, class Res = decltype(std::declval<U>()++)>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class U>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };


        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class op_predecrement
        {
            template<class U, class Res = decltype(--std::declval<U>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class U>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };


        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class op_postdecrement
        {
            template<class U, class Res = decltype(std::declval<U>()--)>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class U>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };


        // -----------------------------------------------------------------------------
        // Comparison operators
        // -----------------------------------------------------------------------------

        template<class T, class Tester = meta::constant_fn<std::true_type>>
        class op_negation
        {
            template<class U, class Res = decltype(!std::declval<U>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class U>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<T>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_equal_to
        {
            template<class L, class R, class Res = decltype(std::declval<L>() == std::declval<R>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_unequal_to
        {
            template<class L, class R, class Res = decltype(std::declval<L>() != std::declval<R>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_lesser
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>() < std::declval<Rhs>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_lesser_equal
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>() <= std::declval<Rhs>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_greater
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>() > std::declval<Rhs>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_greater_equal
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>() >= std::declval<Rhs>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        // -----------------------------------------------------------------------------
        // Arithmetic operators
        // -----------------------------------------------------------------------------

        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_plus
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>() + std::declval<Rhs>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_minus
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>() - std::declval<Rhs>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };

        // -----------------------------------------------------------------------------
        // Compound assignment operators
        // -----------------------------------------------------------------------------

        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_assign
        {
            template<class L, class R, class Res = decltype(std::declval<L>() = std::declval<R>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_plus_assign
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>() += std::declval<Rhs>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };


        template<class Lhs, class Rhs, class Tester = meta::constant_fn<std::true_type>>
        class op_minus_assign
        {
            template<class L, class R, class Res = decltype(std::declval<Lhs>() -= std::declval<Rhs>())>
            static auto test(int) -> meta::invoke<Tester, Res>;
            template<class, class>
            static auto test(...) -> std::false_type;

        public:
            static constexpr bool value = decltype(test<Lhs, Rhs>(0))::value;
        };
    } // namespace test

} // namespace meta


QS_NAMESPACE_END

#endif // QS_META_H
