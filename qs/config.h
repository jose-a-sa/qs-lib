#ifndef QS_CONFIG_H
#define QS_CONFIG_H

#include <cstddef> // requires for _HAS_EXCEPTIONS on MSVC
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <type_traits>
#include <version>
#include <initializer_list>


#define QS_NAMESPACE qs

#define QS_NAMESPACE_BEGIN                                                                                             \
    namespace QS_NAMESPACE                                                                                             \
    {
#define QS_NAMESPACE_END }

// Stringify
#define QS_STRINGIFY(v) #v
// Concatenation
#define QS_CONCAT(A, B) A##B

// #define QS_COMPILER_VERSION_CONCAT(major, minor, patch) (10 * (10 * (major) + (minor)) + (patch))
#define QS_COMPILER_VERSION_CONCAT(major, minor, patch) ((100 * (major)) + (minor))
// Define a in between macro
#define QS_IN_BETWEEN(x, a, b) ((a) <= (x) && (x) < (b))

// Detect compiler versions.
#if defined(__clang__) && !defined(__ibmxl__)
#define QS_CLANG_VERSION QS_COMPILER_VERSION_CONCAT(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
#define QS_CLANG_VERSION 0
#endif
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#define QS_GCC_VERSION QS_COMPILER_VERSION_CONCAT(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
#define QS_GCC_VERSION 0
#endif
#if defined(__ICL)
#define QS_ICC_VERSION __ICL
#elif defined(__INTEL_COMPILER)
#define QS_ICC_VERSION __INTEL_COMPILER
#else
#define QS_ICC_VERSION 0
#endif
#if defined(_MSC_VER)
#define QS_MSVC_VERSION _MSC_VER
#else
#define QS_MSVC_VERSION 0
#endif

// Detect CPU Families
// Detect PowerPC (32-bit & 64-bit)
#if defined(__powerpc__) || defined(__PPC__) || defined(__ppc__)
#define QS_POWERPC 1
#else
#define QS_POWERPC 0
#endif
#if defined(__powerpc64__) || defined(__PPC64__) || defined(__ppc64__)
#define QS_POWERPC64 1
#else
#define QS_POWERPC64 0
#endif
// Detect ARM (32-bit & 64-bit)
#if defined(__arm__) || defined(_M_ARM)
#define QS_ARM 1
#else
#define QS_ARM 0
#endif
#if defined(__aarch64__) || defined(_M_ARM64)
#define QS_ARM64 1
#else
#define QS_ARM64 0
#endif
#if defined(__APPLE__) && QS_ARM64
#define QS_ARM64_APPLE 1
#else
#define QS_ARM64_APPLE 0
#endif
// Detect x86 (32-bit & 64-bit)
#if defined(__x86_64__) || defined(_M_X64)
#define QS_X86_64 1
#else
#define QS_X86_64 0
#endif
#if defined(__i386__) || defined(_M_IX86)
#define QS_X86 1
#else
#define QS_X86 0
#endif

// Detect build type
#ifdef QS_DEBUG
// Use the provided definition.
#elif defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
#define QS_DEBUG 1
#else
#define QS_DEBUG 0
#endif

// Detect standard library versions.
#ifdef _GLIBCXX_RELEASE
#define QS_GLIBCXX_VERSION _GLIBCXX_RELEASE
#else
#define QS_GLIBCXX_VERSION 0
#endif
#ifdef _LIBCPP_VERSION
#define QS_LIBCPP_VERSION _LIBCPP_VERSION
#else
#define QS_LIBCPP_VERSION 0
#endif

#ifdef _MSVC_LANG
#define QS_CPLUSPLUS _MSVC_LANG
#else
#define QS_CPLUSPLUS __cplusplus
#endif

#if QS_CPLUSPLUS >= 202302L
#define QS_STD_VERSION 23
#elif QS_CPLUSPLUS >= 202002L
#define QS_STD_VERSION 20
#elif QS_CPLUSPLUS >= 201703L
#define QS_STD_VERSION 17
#elif QS_CPLUSPLUS >= 201402L
#define QS_STD_VERSION 14
#else
#define QS_STD_VERSION 11
#endif

// Detect __has_*.
#ifdef __has_builtin
#define QS_HAS_BUILTIN(x) __has_builtin(x)
#else
#define QS_HAS_BUILTIN(x) 0
#endif
#ifdef __has_feature
#define QS_HAS_FEATURE(x) __has_feature(x)
#else
#define QS_HAS_FEATURE(x) 0
#endif
#ifdef __has_include
#define QS_HAS_INCLUDE(x) __has_include(x)
#else
#define QS_HAS_INCLUDE(x) 0
#endif
#ifdef __has_cpp_attribute
#define QS_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define QS_HAS_CPP_ATTRIBUTE(x) 0
#endif
#define QS_HAS_CPP14_ATTRIBUTE(attribute) (QS_STD_VER >= 14 && QS_HAS_CPP_ATTRIBUTE(attribute))
#define QS_HAS_CPP17_ATTRIBUTE(attribute) (QS_STD_VER >= 17 && QS_HAS_CPP_ATTRIBUTE(attribute))


// Detect C++11 constexpr.
#ifdef QS_USE_CONSTEXPR11
// Use the provided definition.
#elif QS_CLANG_VERSION && QS_HAS_FEATURE(cxx_constexpr)
#define QS_USE_CONSTEXPR11 1
#elif QS_CLANG_VERSION >= 406
#define QS_USE_CONSTEXPR11 1
#elif QS_MSVC_VERSION >= 1900 // MSVC 2015+
#define QS_USE_CONSTEXPR11 1
#else
#define QS_USE_CONSTEXPR11 0
#endif
#if QS_USE_CONSTEXPR11
#define QS_CONSTEXPR11 constexpr
#else
#define QS_CONSTEXPR11
#endif

// Detect C++14 relaxed constexpr.
#ifdef QS_USE_CONSTEXPR14
// Use the provided definition.
#elif QS_GCC_VERSION >= 600 && QS_CPLUSPLUS >= 201402L
#define QS_USE_CONSTEXPR14 1
#elif QS_ICC_VERSION
#define QS_USE_CONSTEXPR14 0
#elif QS_HAS_FEATURE(cxx_relaxed_constexpr) || QS_MSVC_VERSION >= 1912
#define QS_USE_CONSTEXPR14 1
#else
#define QS_USE_CONSTEXPR14 0
#endif
#if QS_USE_CONSTEXPR14
#define QS_CONSTEXPR14 constexpr
#else
#define QS_CONSTEXPR14
#endif

// Detect C++17 constexpr support
#ifdef QS_USE_CONSTEXPR17
// Use the provided definition.
#elif QS_GCC_VERSION >= 700 && QS_CPLUSPLUS >= 201703L
#define QS_USE_CONSTEXPR17 1
#elif QS_ICC_VERSION
#define QS_USE_CONSTEXPR17 0
#elif QS_HAS_FEATURE(cxx_constexpr_if) || QS_MSVC_VERSION >= 1914
#define QS_USE_CONSTEXPR17 1
#else
#define QS_USE_CONSTEXPR17 0
#endif
#if QS_USE_CONSTEXPR17
#define QS_CONSTEXPR17 constexpr
#else
#define QS_CONSTEXPR17
#endif

// Detect consteval, C++20 constexpr extensions and std::is_constant_evaluated.
#if !defined(__cpp_lib_is_constant_evaluated)
#define QS_USE_CONSTEVAL 0
#elif QS_CPLUSPLUS < 201709L
#define QS_USE_CONSTEVAL 0
#elif QS_GLIBCXX_RELEASE && QS_GLIBCXX_RELEASE < 10
#define QS_USE_CONSTEVAL 0
#elif QS_LIBCPP_VERSION && QS_LIBCPP_VERSION < 10000
#define QS_USE_CONSTEVAL 0
#elif defined(__apple_build_version__) && __apple_build_version__ < 14000029L
#define QS_USE_CONSTEVAL 0 // consteval is broken in Apple clang < 14.
#elif QS_MSVC_VERSION && QS_MSVC_VERSION < 1929
#define QS_USE_CONSTEVAL 0 // consteval is broken in MSVC VS2019 < 16.10.
#elif defined(__cpp_consteval)
#define QS_USE_CONSTEVAL 1
#elif QS_GCC_VERSION >= 1002 || QS_CLANG_VERSION >= 1101
#define QS_USE_CONSTEVAL 1
#else
#define QS_USE_CONSTEVAL 0
#endif
#if QS_USE_CONSTEVAL
#define QS_CONSTEVAL consteval
#define QS_CONSTEXPR20 constexpr
#else
#define QS_CONSTEVAL
#define QS_CONSTEXPR20
#endif


#ifdef QS_EXCEPTIONS
// Use the provided definition.
#elif defined(__cpp_exceptions) || defined(__EXCEPTIONS) || _HAS_EXCEPTIONS
#define QS_EXCEPTIONS 1
#else
#define QS_EXCEPTIONS 0
#endif
#if QS_EXCEPTIONS
#define QS_TRY try
#define QS_CATCH(x) catch(x)
#else
#define QS_TRY if(true)
#define QS_CATCH(x) if(false)
#endif


#if QS_HAS_CPP17_ATTRIBUTE(fallthrough)
#define QS_FALLTHROUGH [[fallthrough]]
#elif QS_CLANG_VERSION
#define QS_FALLTHROUGH [[clang::fallthrough]]
#elif QS_GCC_VERSION >= 700 && (!defined(__EDG_VERSION__) || __EDG_VERSION__ >= 520)
#define QS_FALLTHROUGH [[gnu::fallthrough]]
#else
#define QS_FALLTHROUGH
#endif

// Disable [[noreturn]] on MSVC/NVCC because of bogus unreachable code warnings.
#if QS_HAS_CPP_ATTRIBUTE(noreturn) && !QS_MSVC_VERSION && !defined(__NVCC__)
#define QS_NORETURN [[noreturn]]
#else
#define QS_NORETURN
#endif

#ifndef QS_NODISCARD
#if QS_HAS_CPP17_ATTRIBUTE(nodiscard)
#define QS_NODISCARD [[nodiscard]]
#else
#define QS_NODISCARD
#endif
#endif

#ifdef QS_DEPRECATED
// Use the provided definition.
#elif QS_HAS_CPP14_ATTRIBUTE(deprecated)
#define QS_DEPRECATED [[deprecated]]
#define QS_DEPRECATED_F(msg) [[deprecated(msg)]]
#else
#define QS_DEPRECATED
#define QS_DEPRECATED_F(...)
#endif

#ifdef QS_ALWAYS_INLINE
// Use the provided definition.
#elif QS_GCC_VERSION || QS_CLANG_VERSION
#define QS_ALWAYS_INLINE inline __attribute__((always_inline))
#elif QS_MSVC_VERSION
#define QS_ALWAYS_INLINE __forceinline
#else
#define QS_ALWAYS_INLINE inline
#endif

#ifdef QS_INLINE
// A version of QS_INLINE to prevent code bloat in debug mode.
#elif QS_DEBUG
#define QS_INLINE QS_ALWAYS_INLINE
#else
#define QS_INLINE inline
#endif

#ifdef QS_INLINE_VAR
// Use the provided definition
#elif defined(__cpp_inline_variables)
#define QS_INLINE_VAR inline
#else
#define QS_INLINE_VAR
#endif

#ifdef QS_INLINE_VAR_INIT
// Use the provided definition
#elif defined(__cpp_inline_variables)
#define QS_INLINE_VAR_INIT(...) __VA_ARGS__
#else
#define QS_INLINE_VAR_INIT(...)
#endif

#if QS_GCC_VERSION || QS_CLANG_VERSION
#define QS_VISIBILITY(value) __attribute__((visibility(value)))
#else
#define QS_VISIBILITY(value)
#endif


#ifdef QS_LIKELY
// Use the provided definition.
#elif QS_GCC_VERSION || QS_CLANG_VERSION
#define QS_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define QS_LIKELY(x) (x)
#endif

#ifdef QS_UNLIKELY
// Use the provided definition.
#elif QS_GCC_VERSION || QS_CLANG_VERSION
#define QS_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define QS_UNLIKELY(x) (x)
#endif

#ifdef QS_MAYBE_UNUSED
// Use the provided definition.
#elif QS_HAS_CPP17_ATTRIBUTE(maybe_unused)
#define QS_MAYBE_UNUSED [[maybe_unused]]
#else
#define QS_MAYBE_UNUSED
#endif


#ifdef QS_CACHELINE_SIZE
// Use the provided definition
#elif defined(__cpp_lib_hardware_interference_size)
#include <new> // requires for std::hardware_destructive_interference_size
#define QS_CACHELINE_SIZE std::hardware_destructive_interference_size
#elif (QS_X86_64 || QS_X86)
#define QS_CACHELINE_SIZE 64 // x86 and x86_64 CPUs (Intel & AMD)
#elif QS_ARM64_APPLE
#define QS_CACHELINE_SIZE 128 // Apple Sillicon chips
#elif (QS_ARM64 || QS_ARM)
#define QS_CACHELINE_SIZE 64 // ARM 64-bit (except Apple) and 32-bit (default assumption)
#elif QS_POWERPC64
#define QS_CACHELINE_SIZE 128 // PowerPC64 (IBM POWER9/10)
#elif QS_POWERPC
#define QS_CACHELINE_SIZE 64 // PowerPC 32-bit (default)
#else
#define QS_CACHELINE_SIZE 64 // Default assumption
#endif


QS_NAMESPACE_BEGIN

namespace intl
{
    template<class... Args>
    QS_ALWAYS_INLINE QS_CONSTEXPR17 void ignore_unused(Args&&...)
    {}

    template<class... Args>
    QS_ALWAYS_INLINE QS_CONSTEXPR17 void swallow(Args&&... args)
    {
        int dummy[] = {0, (static_cast<void>(args), 0)...};
        ignore_unused(dummy);
    }

    template<class... Args>
    struct make_void
    {
        using type = void;
    };

    template<template<class...> class Fn, class... Args, class = Fn<Args...>>
    static constexpr auto is_valid_expansion_impl(int) -> std::true_type;
    template<template<class...> class, class...>
    static constexpr auto is_valid_expansion_impl(...) -> std::false_type;


} // namespace intl


enum class contract_violation_mode
{
    ignore    = 0,
    exception = 1,
    terminate = 2
};

template<contract_violation_mode Mode>
struct contract_violation;

template<>
struct contract_violation<contract_violation_mode::ignore>
{
    QS_ALWAYS_INLINE static QS_CONSTEXPR17 void fail(char const*, int, char const*) noexcept {}
};
template<>
struct contract_violation<contract_violation_mode::exception>
{
    QS_NORETURN QS_ALWAYS_INLINE static void fail(char const* file, int line, char const* msg)
    {
        intl::ignore_unused(file, line);
        throw std::logic_error(msg);
    }
};
template<>
struct contract_violation<contract_violation_mode::terminate>
{
    QS_NORETURN QS_ALWAYS_INLINE static QS_CONSTEXPR17 void fail(char const* file, int line, char const* msg) noexcept
    {
        std::fprintf(stderr, "[%s:%d] assert_fail: %s\n", file, line, msg);
        std::abort();
    }
};

namespace config
{
    QS_INLINE_VAR static QS_CONSTEXPR11 unsigned cpp_std_version = (QS_STD_VERSION);
    QS_INLINE_VAR static QS_CONSTEXPR11 unsigned cache_line_size = (QS_CACHELINE_SIZE);
    QS_INLINE_VAR static QS_CONSTEXPR11 bool     has_exceptions  = (QS_EXCEPTIONS == 1);
    QS_INLINE_VAR static QS_CONSTEXPR11 bool     debug_mode      = (QS_DEBUG == 1);

    // QS_INLINE_VAR static QS_CONSTEXPR11 contract_violation_mode contract_violation_mode =
    //     has_exceptions ? contract_violation_mode::exception : contract_violation_mode::terminate;

    QS_INLINE_VAR static QS_CONSTEXPR11 contract_violation_mode contract_violation_mode =
        contract_violation_mode::terminate;
} // namespace config



template<class... Args>
using void_t = typename intl::make_void<Args...>::type;

template<bool B, class T = void>
struct enable_if : std::enable_if<B, T>
{};
template<bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template<bool B, class T, class F>
using conditional_t = typename std::conditional<B, T, F>::type;

template<class T>
struct decay : std::decay<T>
{};
template<class T>
using decay_t = typename std::decay<T>::type;


template<template<class...> class Fn, class... Args>
using is_valid_expansion = decltype(intl::is_valid_expansion_impl<Fn, Args...>(0));


template<class T>
using remove_reference_t = typename std::remove_reference<T>::type;
template<class T>
using remove_const_t = typename std::remove_const<T>::type;
template<class T>
using remove_cv_t = typename std::remove_cv<T>::type;
template<class T>
struct remove_cvref : std::remove_cv<remove_reference_t<T>>
{};
template<class T>
using remove_cvref_t = typename remove_cvref<T>::type;
template<class T>
using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;
template<class T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;
template<class T>
using add_const_t = typename std::add_const<T>::type;
template<class T>
using add_pointer_t = typename std::add_pointer<T>::type;
template<class T>
using make_cref_t = add_lvalue_reference_t<add_const_t<remove_reference_t<T>>>;

template<class T>
struct type_identity
{
    using type = T;
};
template<class T>
using type_identity_t = typename type_identity<T>::type;

template<class... Ts>
using common_type_t = typename std::common_type<Ts...>::type;


template<class... Args>
struct disjunction;
template<>
struct disjunction<> : std::false_type
{};
template<class B1>
struct disjunction<B1> : B1
{};
template<class B1, class... Rest>
struct disjunction<B1, Rest...> : conditional_t<B1::value, std::true_type, disjunction<Rest...>>
{};


template<class... Args>
struct conjunction;
template<>
struct conjunction<> : std::true_type
{};
template<class B1>
struct conjunction<B1> : B1
{};
template<class B1, class... Rest>
struct conjunction<B1, Rest...> : conditional_t<!B1::value, std::false_type, conjunction<Rest...>>
{};

template<class B1>
struct negation : std::integral_constant<bool, !B1::value>
{};


QS_INLINE QS_CONSTEXPR17 bool is_constant_evaluated(bool const default_value = false) noexcept
{
#if defined(__cpp_lib_is_constant_evaluated)
    intl::ignore_unused(default_value);
    return std::is_constant_evaluated();
#elif QS_HAS_BUILTIN(__builtin_is_constant_evaluated) || QS_GCC_VERSION >= 900 || QS_MSVC_VERSION >= 1928
    intl::ignore_unused(default_value);
    return __builtin_is_constant_evaluated();
#else
    return default_value;
#endif
}


// -----------------------------------------------------------------------------
// size, data, empty, ssize
// -----------------------------------------------------------------------------

// std::data
template<class C>
constexpr auto data(C& c) noexcept(noexcept(c.data())) -> decltype(c.data())
{
    return c.data();
}
template<class C>
constexpr auto data(C const& c) noexcept(noexcept(c.data())) -> decltype(c.data())
{
    return c.data();
}
template<class T, std::size_t N>
constexpr T* data(T (&array)[N]) noexcept
{
    return array;
}
template<class T>
constexpr T const* data(std::initializer_list<T> il) noexcept
{
    return il.begin();
}

// std::size
template<class C>
constexpr auto size(C const& c) -> decltype(c.size())
{
    return c.size();
}
template<class T, size_t N>
constexpr size_t size(T (&)[N]) noexcept
{
    return N;
}

// std::empty
template<class C>
QS_NODISCARD constexpr auto empty(C const& c) -> decltype(c.empty())
{
    return c.empty();
}
template<class T, size_t N>
QS_NODISCARD constexpr bool empty(const T (&array)[N]) noexcept
{
    intl::ignore_unused(array);
    return false;
}
template<class T>
QS_NODISCARD constexpr bool empty(std::initializer_list<T> il) noexcept
{
    return il.size() == 0;
}

// std::ssize
template<class C>
constexpr auto ssize(C const& c) -> common_type_t<ptrdiff_t, typename std::make_signed<decltype(c.size())>::type>
{
    using R = common_type_t<ptrdiff_t, typename std::make_signed<decltype(c.size())>::type>;
    return static_cast<R>(c.size());
}
template<class T, ptrdiff_t N>
constexpr ptrdiff_t ssize(const T (&array)[N]) noexcept
{
    intl::ignore_unused(array);
    return N;
}


// -----------------------------------------------------------------------------
// Assertions and Contracts
// -----------------------------------------------------------------------------


QS_INLINE_VAR static QS_CONSTEXPR11 bool is_nothrow_contract_violation =
    noexcept(contract_violation<config::contract_violation_mode>::fail("", 0, ""));


#ifdef QS_ASSERT
// Use the provided definition.
#elif !QS_DEBUG
#define QS_ASSERT(condition, message) (QS_NAMESPACE::intl::ignore_unused)((condition), (message)) // Avoid -Wempty-body.
#else
#define QS_ASSERT(condition, message)                                                                                  \
    (QS_LIKELY(condition) ? ((QS_NAMESPACE::intl::ignore_unused)(message))                                             \
                          : ((QS_NAMESPACE::contract_violation<QS_NAMESPACE::config::contract_violation_mode>::fail)(  \
                                __FILE_NAME__, __LINE__, message)))
#endif

// clang-format off
#ifdef QS_ASSUME
// Use the provided definition.
#elif defined(__CUDACC__) && defined(__CUDA_ARCH__)
#define QS_ASSUME(condition) do { if (!(condition)) __builtin_unreachable(); } while (0)
#elif QS_HAS_BUILTIN(__builtin_assume) || QS_GCC_VERSION >= 900 || defined(__NVCOMPILER)
#define QS_ASSUME(condition) __builtin_assume(condition)
#elif QS_MSVC_VERSION || defined(__INTEL_COMPILER) || defined(__INTEL_LLVM_COMPILER)
#define QS_ASSUME(condition) __assume(condition)
#elif defined(__cpp_lib_assume)
#include <utility>
#define QS_ASSUME(condition) std::assume(condition)
#else
#define QS_ASSUME(condition) ((QS_NAMESPACE::intl::ignore_unused)(condition))
#endif
// clang-format on


#ifdef QS_VERIFY
// Use the provided definition.
#else
#define QS_VERIFY(cond, msg) QS_ASSERT(cond, msg)
#endif


QS_NAMESPACE_END


#endif
