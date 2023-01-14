#ifndef GNTST_BASE_H
#define GNTST_BASE_H

/* size_t bool */
#if defined(__cplusplus)
#  include <cstddef>
#else
#  include <stddef.h>
#  if (__STDC_VERSION__ >= 199901L)
#    include <stdbool.h>
#  elif !defined(bool)
#    define bool char
#    define true 1
#    define false 0
#  endif
#endif

#define NTST_STRINGIZE(x) #x
#define NTST_STRINGIZE_MACRO(x) NTST_STRINGIZE(x)

#if defined(__cplusplus)
#  if (__cplusplus >= 201103L) || (_MSC_VER >= 1900)
#    define NTST_CONSTEXPR constexpr
#  else
#    define NTST_CONSTEXPR
#  endif
template <typename ElemType, size_t arr_size>
NTST_CONSTEXPR size_t NTST_ARRAY_LEN(ElemType const (&)[arr_size]) {return arr_size;}
#else
#  define NTST_ARRAY_LEN(array) (sizeof(array)/sizeof(*array))
#endif

#if defined(__cplusplus)
template <typename ElemType, size_t arr_size>
NTST_CONSTEXPR size_t NTST_STR_LEN(ElemType const (&)[arr_size]) {return arr_size - 1;}
#else
#  define NTST_STR_LEN(array) (sizeof(array)/sizeof(*array) - 1)
#endif

/* NTST_NOEXCEPT */
#if defined(__cplusplus)
#  if (__cplusplus >= 201103L)
#    define NTST_NOEXCEPT noexcept
#  else
#    define NTST_NOEXCEPT throw()
#  endif
#else
#  define NTST_NOEXCEPT
#endif

/* Branch prediction */
#if defined(__GNUC__) || defined(__clang__)
#define NTST_LIKELY(expr) __builtin_expect(!!(expr),1)
#define NTST_UNLIKELY(expr) __builtin_expect(!!(expr),0)
#elif defined(__has_cpp_attribute)
#  if __has_cpp_attribute(likely)
#  define NTST_EMPTY__()
#  define NTST_LIKELY(expr) expr) [[likely]] NTST_EMPTY__(
#  define NTST_UNLIKELY(expr) expr) [[unlikely]] NTST_EMPTY__(
#  endif
#endif

#if !defined(NTST_LIKELY)
#define NTST_LIKELY(expr) (expr)
#define NTST_UNLIKELY(expr) (expr)
#endif

/* NTST_RESTRICT */
#if defined(_MSC_VER)
#  define NTST_RESTRICT __restrict
#elif defined(__GNUC__)
#  define NTST_RESTRICT __restrict__
#elif (__STDC_VERSION__ >= 199901L)
#  define NTST_RESTRICT restrict
#else
#  define NTST_RESTRICT
#endif
/* NTST_RESTRICT end */

/* static_assert */
#if (__cplusplus >= 201103L) || defined(__cpp_static_assert) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#  define ntst_static_assert(a, b) static_assert(a, b)
#elif (__STDC_VERSION__ >= 201112L)
#  define ntst_static_assert(a, b) _Static_assert(a, b)
#elif (defined(_MSC_VER)) && (_MSC_VER >= 1600)
#  define ntst_static_assert(a, b) static_assert(a, b)
#else
#  define ntst_static_assert(a, b)
#  define NTST_NO_STATIC_ASSERT 1
#endif

#if (__cplusplus >= 201103L)
#  define NTST_NORETURN [[noreturn]]
#elif (__STDC_VERSION__ >= 201112L)
#  define NTST_NORETURN _Noreturn
#elif defined(__GNUC__)
#  define NTST_NORETURN __attribute__((__noreturn__))
#elif defined(_MSC_VER)
#  define NTST_NORETURN __declspec(noreturn)
#else
#  define NTST_NORETURN
#endif

#if (__cplusplus >= 201103L)
#  define NTST_HAS_RVALUE_REF 1
#elif (_MSC_VER >= 1600)
#  define NTST_HAS_RVALUE_REF 1
#elif defined(__clang__)
#  if __has_feature(cxx_rvalue_references)
#    define NTST_HAS_RVALUE_REF 1
#  endif
/* compiler version is not correct solution when old standard selected(-std=c++03) (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 3) */
#elif defined(__GXX_EXPERIMENTAL_CXX0X__)
#  define NTST_HAS_RVALUE_REF 1
#endif


/* final override */
#if (__cplusplus >= 201103L)
#  define NTST_FINAL final
#  define NTST_OVERRIDE override
#elif (_MSC_VER >= 1700)
#  define NTST_FINAL final
#  define NTST_OVERRIDE override
#else
#  define NTST_FINAL
#  define NTST_OVERRIDE
#endif

#ifndef NTST_THROW_BAD_ALLOC
#define NTST_THROW_BAD_ALLOC throw std::bad_alloc()
#endif

#if defined(__clang__) || defined(__GNUC__)
#define NTST_NONNULL_ARGS __attribute__((nonnull))
#define NTST_NONNULL_ARGS_LIST(...) __attribute__((nonnull(__VA_ARGS__)))
#else
#define NTST_NONNULL_ARGS
#define NTST_NONNULL_ARGS_LIST(...)
#endif

#if defined(__clang__) || defined(__GNUC__)
#define NTST_ATTRIB_PRINTF_FMT(frmt,frst) __attribute__((__format__(__printf__,frmt,frst)))
#else
#define NTST_ATTRIB_PRINTF_FMT(format,first)
#endif

#ifdef __GNUC__
#define NTST_ALWAYS_INLINE inline __attribute__((__always_inline__))
#elif defined(_MSC_VER)
#define NTST_ALWAYS_INLINE __forceinline
#elif defined(__CLANG__)
#if __has_attribute(__always_inline__)
    #define NTST_ALWAYS_INLINE inline __attribute__((__always_inline__))
#else
    #define NTST_ALWAYS_INLINE inline
#endif
#else
#define NTST_ALWAYS_INLINE inline
#endif

/* #ifndef GNTST_BASE_H */
#endif
