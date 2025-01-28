#ifndef QS_BASE_H_
#define QS_BASE_H_

#include <qs/config.h>


#include <memory>
#include <type_traits>


#define QS_CONFIG_ADD_CPP20_TRAITS 1
#define QS_CONFIG_ADD_CPP23_TRAITS 1

#define QS_CONFIG_ADD_CPP20_MEMORY 1
#define QS_CONFIG_ADD_CPP23_MEMORY 1


#include <cassert>

#define QS_ASSERT(x, ...) assert(x)


QS_NAMESPACE_BEGIN


namespace config
{
    static QS_INLV_CONSTEXPR11 unsigned cpp_version  = (QS_CPP_VERSION); // Store the C++ version
    static QS_INLV_CONSTEXPR11 bool     has_noexcept = (QS_HAS_NO_EXCEPTIONS); // Check if noexcept is supported
} // namespace config


//
//
// C++20 type traits not available in C++17
//
//
#if QS_HAS_CPP(20)

using std::is_bounded_array; // C++20
using std::is_unbounded_array; // C++20
using std::remove_cvref; // C++20
using std::type_identity; // C++20

#elif QS_CONFIG(ADD_CPP20_TRAITS)
// Implementations of C++20 type traits for C++17

template<class T>
struct remove_cvref : std::remove_cv<std::remove_reference_t<T>>
{};

template<class>
struct is_bounded_array : std::false_type
{};
template<class T, size_t N>
struct is_bounded_array<T[N]> : std::true_type
{};

template<class>
struct is_unbounded_array : std::false_type
{};
template<class T>
struct is_unbounded_array<T[]> : std::true_type
{};

template<class T>
struct type_identity
{
    using type = T;
};

#endif // QS_CONFIG(ADD_CPP20_TRAITS)


#if QS_CONFIG(ADD_CPP20_TRAITS) || QS_HAS_CPP(20)

    template<class T>
    using remove_cvref_t = typename remove_cvref<T>::type; // C++20

template<class T>
using type_identity_t = typename type_identity<T>::type; // C++20

#if QS_HAS(VARIABLE_TEMPLATES)

template<class T>
QS_INLINE_VAR constexpr bool is_bounded_array_v = is_bounded_array<T>::value; // C++20
template<class T>
QS_INLINE_VAR constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value; // C++20

#endif // QS_HAS(VARIABLE_TEMPLATES)

#endif // QS_HAS(VARIABLE_TEMPLATES) && (QS_CONFIG(ADD_CPP20_TRAITS) || QS_HAS_CPP(20))


//
//
// C++20 memory utils (to_address) not available in C++17
//
//
#if QS_HAS_CPP(20)

using std::make_unique_for_overwrite;
using std::to_address;

#elif QS_CONFIG(ADD_CPP20_MEMORY)
// Implementations of C++20 memory utilities for C++17

template<class T>
QS_CONSTEXPR23 std::enable_if_t<!std::is_array<T>::value, std::unique_ptr<T>> make_unique_for_overwrite()
{
    return std::unique_ptr<T>(new T);
}

template<class T>
QS_CONSTEXPR23 std::enable_if_t<is_unbounded_array<T>::value, std::unique_ptr<T>> make_unique_for_overwrite(size_t n)
{
    return std::unique_ptr<T>(new std::remove_extent_t<T>[n]);
}

template<class T, class... Args>
std::enable_if_t<is_bounded_array<T>::value> make_unique_for_overwrite(Args&&...) = delete;


namespace detail
{
    template<class Pointer, class = void>
    struct is_smart_pointer : std::false_type
    {};

    template<class Pointer>
    struct is_smart_pointer<
        Pointer, std::void_t<decltype(std::declval<Pointer const&>().operator->()),
                             decltype(std::pointer_traits<Pointer>::to_address(std::declval<Pointer const&>()))>>
        : std::true_type
    {};

    template<class Pointer, class = void>
    struct to_address_helper;

    template<class Pointer, class>
    struct to_address_helper
    {
        using result_type = decltype(to_address(std::declval<Pointer const&>().operator->()));
        static constexpr result_type call(Pointer const& p) noexcept { return to_address(p.operator->()); }
    };

    template<class Pointer>
    struct to_address_helper<
        Pointer, std::void_t<decltype(std::pointer_traits<Pointer>::to_address(std::declval<Pointer const&>()))>>
    {
        using result_type = decltype(std::pointer_traits<Pointer>::to_address(std::declval<Pointer const&>()));
        static constexpr result_type call(Pointer const& p) noexcept
        {
            return std::pointer_traits<Pointer>::to_address(p);
        }
    };

    // enable_if is needed here to avoid instantiating checks for fancy pointers on raw pointers
    template<class Pointer,
             class = std::enable_if_t<std::conjunction<std::is_class<Pointer>, is_smart_pointer<Pointer>>::value>>
    inline QS_CONSTEXPR11 std::decay_t<typename to_address_helper<Pointer>::result_type>
                          to_address_impl(Pointer const& p) QS_NOEXCEPT
    {
        return to_address_helper<Pointer>::call(p);
    }
} // namespace detail

template<class T>
inline QS_CONSTEXPR11 auto to_address(T* p) QS_NOEXCEPT -> decltype(detail::to_address_impl(p))
{
    return detail::to_address_impl(p);
}

template<class Pointer>
inline QS_CONSTEXPR11 auto to_address(Pointer const& p) QS_NOEXCEPT -> decltype(detail::to_address_impl(p))
{
    return detail::to_address_impl(p);
}
#endif


QS_NAMESPACE_END


#endif // QS_BASE_H_
