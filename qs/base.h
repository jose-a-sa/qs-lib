#ifndef QS_BASE_H_
#define QS_BASE_H_

#include <qs/config.h>

#include <memory>
#include <type_traits>


#define QS_CONFIG_ADD_CPP20_TRAITS 1
#define QS_CONFIG_ADD_CPP23_TRAITS 1

#define QS_CONFIG_ADD_CPP17_MEMORY 1
#define QS_CONFIG_ADD_CPP20_MEMORY 1
#define QS_CONFIG_ADD_CPP23_MEMORY 1


QS_NAMESPACE_BEGIN

namespace config
{
    QS_INLINE_VAR static QS_CONSTEXPR11 unsigned cpp_version = (QS_CPP_VERSION); // Store the C++ version
    QS_INLINE_VAR static QS_CONSTEXPR11 bool   has_noexcept  = (QS_HAS_NO_EXCEPTIONS); // Check if noexcept is supported
    QS_INLINE_VAR static QS_CONSTEXPR11 size_t cache_line_size = (QS_CACHELINE_SIZE); // Cache line size
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


template<class T>
struct remove_cvref : std::remove_cv<std::remove_reference_t<T>>
{};
template<class T>
using remove_cvref_t = typename remove_cvref<T>::type; // C++20


template<class>
struct is_bounded_array : std::false_type
{};
template<class T, size_t N>
struct is_bounded_array<T[N]> : std::true_type
{};
#if defined(__cpp_inline_variables)
template<class T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value; // C++20
#endif


template<class>
struct is_unbounded_array : std::false_type
{};
template<class T>
struct is_unbounded_array<T[]> : std::true_type
{};
#if defined(__cpp_inline_variables)
template<class T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value; // C++20
#endif


template<class T>
struct type_identity
{
    using type = T;
};
template<class T>
using type_identity_t = typename type_identity<T>::type; // C++20


#endif


//
//
// C++20 memory utils (to_address) not available in C++17
//
//
#if QS_HAS_CPP(17)

using std::destroy;
using std::destroy_at;

#elif QS_CONFIG(ADD_CPP17_MEMORY)

template<class T, std::enable_if_t<!std::is_array<T>::value, int> = 0>
QS_CONSTEXPR20 void destroy_at(T* location)
{
    QS_ASSERT(location != nullptr, "null pointer location given to destroy_at");
    location->~T();
}

template<class ForwardIterator>
QS_CONSTEXPR20 ForwardIterator destroy(ForwardIterator first, ForwardIterator last)
{
    while(first != last)
        destroy_at(std::addressof(*(first++)));
    return first;
}

#endif


template<class BidirectionalIterator>
QS_CONSTEXPR20 BidirectionalIterator reverse_destroy(BidirectionalIterator first, BidirectionalIterator last)
{
    while(last != first)
        destroy_at(std::addressof(*(--last)));
    return last;
}


#if QS_HAS_CPP(20)

using std::construct_at;
using std::make_unique_for_overwrite;

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


template<class T, class... Args, class = decltype(::new(std::declval<void*>()) T(std::declval<Args>()...))>
QS_CONSTEXPR20 T* construct_at(T* location, Args&&... args)
{
    QS_ASSERT(location != nullptr, "null pointer location given to construct_at");
    return ::new(const_cast<void*>(static_cast<void const volatile*>(location))) T(std::forward<Args>(args)...);
    // return ::new(static_cast<void*>(location)) T(std::forward<Args>(args)...);
}

#endif


QS_NAMESPACE_END


#endif // QS_BASE_H_
