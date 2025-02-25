#ifndef QS_CONFIG_H_
#define QS_CONFIG_H_

#include <qs/macros.h>
#include <version>

/*** UTILITY MACROS ***/

#define QS_MIN_CPP_VERSION 17

#define QS_NAMESPACE qs
#define QS_NAMESPACE_BEGIN                                                                                             \
    namespace QS_NAMESPACE                                                                                             \
    {
#define QS_NAMESPACE_END }


// Check boolean QS_HAS_XXX
#define QS_HAS(feature) (QS_HAS_##feature)
// Check boolean QS_CONFIG_XXX
#define QS_CONFIG(config) (QS_CONFIG_##config)


/*** EXCEPTION HANDLING ***/

#ifndef QS_HAS_NO_EXCEPTIONS
#if defined(_MSC_VER)
#include <cstddef> // for _H_AS_EXCEPTIONS
#endif
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || _H_AS_EXCEPTIONS
#define QS_HAS_NO_EXCEPTIONS 0
#else
#define QS_HAS_NO_EXCEPTIONS 1
#endif
#endif

#if QS_HAS_NO_EXCEPTIONS
#define QS_NOEXCEPT
#define QS_NOEXCEPT_B(...)
#define QS_NOEXCEPT_V(...)
#else
#define QS_NOEXCEPT noexcept
#define QS_NOEXCEPT_B(...) noexcept((__VA_ARGS__))
#define QS_NOEXCEPT_V(...) noexcept(noexcept((__VA_ARGS__)))
#endif


/*** ASSERTION HANDLING ***/

#include <cassert>

#define QS_ASSERT(x, ...) assert(x && (__VA_ARGS__))


/*** COMPILER VARIABLES ***/

// Compiler versions:
//
// MSVC++  6.0  _MSC_VER == 1200  QS_COMPILER_MSVC_VERSION ==  60  (Visual Studio 6.0)
// MSVC++  7.0  _MSC_VER == 1300  QS_COMPILER_MSVC_VERSION ==  70  (Visual Studio .NET 2002)
// MSVC++  7.1  _MSC_VER == 1310  QS_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003)
// MSVC++  8.0  _MSC_VER == 1400  QS_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  QS_COMPILER_MSVC_VERSION ==  90  (Visual Studio 2008)
// MSVC++ 10.0  _MSC_VER == 1600  QS_COMPILER_MSVC_VERSION == 100  (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700  QS_COMPILER_MSVC_VERSION == 110  (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800  QS_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900  QS_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015)
// MSVC++ 14.1  _MSC_VER >= 1910  QS_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  QS_COMPILER_MSVC_VERSION == 142  (Visual Studio 2019)

// Merge version numbers into a single number
#define QS_COMPILER_VERSION_CONCAT(major, minor, patch) (10 * (10 * (major) + (minor)) + (patch))
// Define a in between macro
#define QS_IN_BETWEEN(x, a, b) ((a) <= (x) && (x) <= (b))


#if defined(_MSC_VER) && !defined(__clang__)
#define QS_COMPILER_MSVC_VER (_MSC_VER)
#define QS_COMPILER_MSVC_VERSION (_MSC_VER / 10 - 10 * (5 + (_MSC_VER < 1900)))
#else
#define QS_COMPILER_MSVC_VER 0
#define QS_COMPILER_MSVC_VERSION 0
#endif

#if defined(__clang__)
#define QS_COMPILER_CLANG_VERSION QS_COMPILER_VERSION_CONCAT(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
#define QS_COMPILER_CLANG_VERSION 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define QS_COMPILER_GNUC_VERSION QS_COMPILER_VERSION_CONCAT(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
#define QS_COMPILER_GNUC_VERSION 0
#endif


/*** COMPILER BUILTIN_VARS ***/


/*** CPP VERSION VARIABLES ***/

#ifndef QS_CPLUSPLUS
#if defined(_MSVC_LANG) && !defined(__clang__)
#define QS_CPLUSPLUS (_MSC_VER == 1900 ? 201103L : _MSVC_LANG)
#else
#define QS_CPLUSPLUS __cplusplus
#endif
#endif

// Define C++ version macros
#define QS_HAS_CPP11 (QS_CPLUSPLUS >= 201103L)
#define QS_HAS_CPP14 (QS_CPLUSPLUS >= 201402L)
#define QS_HAS_CPP17 (QS_CPLUSPLUS >= 201703L)
#define QS_HAS_CPP20 (QS_CPLUSPLUS >= 202002L)
#define QS_HAS_CPP23 (QS_CPLUSPLUS >= 202300L)

// Parse version into a number, e.g. 03, 11, 14, 17, 20, 23
#define QS_CPP_VERSION ((QS_CPLUSPLUS / 100) - 2000)
// Define macro to check if in standard version
#define QS_HAS_CPP(v) (QS_CPP_VERSION >= (v))


/*** CPP FEATURES VARIABLES ***/


#if (QS_HAS_CPP(11) || QS_COMPILER_MSVC_VER >= 1900)
#define QS_CONSTEXPR11 constexpr
#else
#define QS_CONSTEXPR11
#endif

#if QS_HAS_CPP(14)
#define QS_CONSTEXPR14 constexpr
#else
#define QS_CONSTEXPR14
#endif

#if QS_HAS_CPP(17)
#define QS_CONSTEXPR17 constexpr
#else
#define QS_CONSTEXPR17
#endif

#if QS_HAS_CPP(20)
#define QS_CONSTEXPR20 constexpr
#else
#define QS_CONSTEXPR20
#endif

#if QS_HAS_CPP(23)
#define QS_CONSTEXPR20 constexpr
#else
#define QS_CONSTEXPR23
#endif


#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(deprecated)
#define QS_DEPRECATED(msg) [[deprecated(msg)]]
#else
#define QS_DEPRECATED(msg)
#endif
#else
#define QS_DEPRECATED(msg)
#endif


#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(nodiscard)
#define QS_NODISCARD [[nodiscard]]
#else
#define QS_NODISCARD
#endif
#else
#define QS_NODISCARD
#endif


#if defined(__cpp_inline_variables)
#define QS_INLINE_VAR inline
#else
#define QS_INLINE_VAR
#endif


#if defined(__cpp_lib_hardware_interference_size)
#define QS_CACHELINE_SIZE std::hardware_destructive_interference_size
#else
#define QS_CACHELINE_SIZE 64
#endif


#endif // QS_CONFIG_H_
