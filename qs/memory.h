#ifndef QS_MEMORY_H
#define QS_MEMORY_H

#include <qs/config.h>

QS_NAMESPACE_BEGIN

#if defined(__cpp_lib_byte)
using std::byte;
#else
enum class byte : unsigned char
{
};
#endif


template<class T>
QS_INLINE QS_CONSTEXPR17 T* launder(T* p) noexcept
{
#if defined(__cpp_lib_launder) // Standard C++17 feature test macro
    return std::launder(p);
#elif QS_HAS_BUILTIN(__builtin_launder) || QS_GCC_VERSION >= 700 || QS_MSVC_VERSION >= 1914
    return __builtin_launder(p);
#else
    return p; // Fallback for pre-C++17 compilers
#endif
}

// Note: maybe replace decltype check with meta::test::placement_new<T, Args...> in the future
template<class T, class... Args, class = decltype(::new(std::declval<void*>()) T(std::declval<Args>()...))>
QS_CONSTEXPR20 T* construct_at(T* location, Args&&... args)
{
    QS_VERIFY(location != nullptr, "null pointer location given to construct_at");
    return ::new(const_cast<void*>(static_cast<void const volatile*>(location))) T(std::forward<Args>(args)...);
    // return ::new(static_cast<void*>(location)) T(std::forward<Args>(args)...);
}

template<class T, enable_if_t<!std::is_array<T>::value, int> = 0>
QS_CONSTEXPR20 void destroy_at(T* location) noexcept
{
    QS_VERIFY(location != nullptr, "null pointer given to destroy_at");
    location->~T();
}

template<class ForwardIterator>
QS_CONSTEXPR20 ForwardIterator destroy(ForwardIterator first, ForwardIterator last) noexcept
{
    while(first != last)
        destroy_at(std::addressof(*(first++)));
    return first;
}

template<class BidirectionalIterator>
QS_CONSTEXPR20 BidirectionalIterator reverse_destroy(BidirectionalIterator first, BidirectionalIterator last) noexcept
{
    while(last != first)
        destroy_at(std::addressof(*(--last)));
    return last;
}


QS_NAMESPACE_END


#endif // QS_MEMORY_H
