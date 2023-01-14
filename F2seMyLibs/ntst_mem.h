#ifndef GNTST_MEM_H
#define GNTST_MEM_H

#include "ntst_base.h"

#include <stdlib.h> /* malloc */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(NTST_MALLOC) && !defined(NTST_FREE) && !defined(NTST_REALLOC)

#define NTST_MALLOC malloc
#define NTST_FREE free
#define NTST_REALLOC realloc

#endif

#if defined(_WIN32)
#include <malloc.h>
#define NTST_MALLOC_SIZE(p) _msize(p)
#elif defined(__linux__)
#include <malloc.h>
#define NTST_MALLOC_SIZE(p) malloc_usable_size(p)
#endif

#ifdef  __GNUC__
#define NTST_MALLOCA(size) __builtin_alloca(size)
#define NTST_FREE_MA(ptr) ((void)0)
#define NTST_ARRAY_ON_STACK(type, name, size) type* name = (type*)__builtin_alloca((size) * sizeof(type))
#elif defined(_MSC_VER)
#include <malloc.h>
#define NTST_MALLOCA(size) _alloca(size)
#define NTST_FREE_MA(ptr) ((void)0)
#define NTST_ARRAY_ON_STACK(type, name, size) type* name = (type*)_alloca((size) * sizeof(type))
#else
#define NTST_MALLOCA(size) NTST_MALLOC(size)
#define NTST_FREE_MA(ptr) NTST_FREE(ptr)
#define NTST_ARRAY_ON_STACK(type, name, size) type name[size]
#endif

#ifdef __cplusplus
}
#endif

#endif
