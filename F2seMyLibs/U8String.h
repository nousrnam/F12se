/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef IG_U8STRING_H_
#define IG_U8STRING_H_

#define U8S_DECL
#define U8S_CALL

#ifdef __cplusplus
#   include <cstddef> /* size_t */
#   if defined(_MSC_VER) || defined(__BORLANDC__)
        typedef __int32 char32;
        typedef unsigned __int32 uchar32;
#   else
#       include <stdint.h>
        typedef int32_t char32;
        typedef uint32_t uchar32;
#   endif
    extern "C" {
#else
#   include <stddef.h> /* size_t */
#   if defined(_MSC_VER) || defined(__BORLANDC__)
        typedef __int32 char32;
        typedef unsigned __int32 uchar32;
#   else
#       include <stdint.h>
        typedef int32_t char32;
        typedef uint32_t uchar32;
#   endif
#endif

/**
 * Error codes
 */
typedef enum U8S_ERR_CODE
    {
    U8S_ERROR_SUCCESS = 0,
    U8S_ERROR_NOT_UTF8, /* Sequence of characters is not UTF8 encoded. */
    U8S_ERROR_ABOVE_UNICODE_LIMIT, /* Code point is illegal(above U8S_UNICODE_LIMIT). */
    U8S_ERROR_MEM_ALLOC_FAIL, /* Memory allocation failed. */
    U8S_ERROR_NULL_PTR, /* NULL pointer is passed as parameter. */
    U8S_ERROR_REFERENCE_MOD, /* Attemt to modify string which is reference to const char* */
    U8S_ERROR_OUT_OF_RANGE, /* Offset or Index is out of range(size or length) */
    U8S_ERROR_ITER_DIF_STRINGS, /* Iterators belong to different strings. */
    U8S_ERROR_NOT_FOUND,
    U8S_ERROR_UNKNOWN
    } U8S_ERR_CODE;


/**
 * UTF8 encoded, null terminated string.
 */
typedef struct U8String
    {
    /**
     * Pointer to UTF8 encoded, null terminated string.
     */
    char *data;

    /**
     * Length of string in code points.
     */
    size_t length;

    /**
     * Number of bytes used for string. Including null terminator.
     * So it must be at least 1 byte(empty string).
     *
     * if (memoryUsed == length + 1) -> this is ASCII string. (c) Captain Obvious
     */
    size_t memoryUsed;

    /**
     * Number of bytes allocated for string. == 0 for const references.
     */
    size_t memoryAllocated;
    } U8String;

/**
 * Iterator for operations with U8String.
 */
typedef struct U8StrIter
    {
    /**
     * Offset in bytes from the start of the string. < memoryUsed
     */
    size_t offset;

    /**
     * Offset in code points from the start of the string. <= length
     */
    size_t position;
    } U8StrIter;

#define U8S_TERMINATOR_SIZE 1
#define U8S_CP_MAX_SIZE_BYTE 4

/* One day these macros may become functions. */
#define u8s_get_length(x)          ((x)->length)
/*#define u8s_get_size(x)            ((x)->memoryUsed >= U8S_TERMINATOR_SIZE ? (x)->memoryUsed - U8S_TERMINATOR_SIZE : 0)*/
#define u8s_get_size(x)            ((x)->memoryUsed - U8S_TERMINATOR_SIZE)
#define u8s_get_memory_used(x)      ((x)->memoryUsed)
#define u8s_get_memory_allocated(x) ((x)->memoryAllocated)
#define u8s_get_cstr(x) ((const char*)(x)->data)

#define u8s_iter_get_cstr(x) ((x)->str->data + (x)->offset)

#define u8s_iter_get_str(x)      ((x).str)
#define u8s_iter_get_offset(x)   ((x).offset)
#define u8s_iter_get_position(x) ((x).position)


U8S_DECL U8S_ERR_CODE U8S_CALL u8s_create(const char *inData, U8String **outStr);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_create_size(const char *inData, size_t size, U8String **outStr);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_create_length(const char *inData, size_t length, U8String **outStr);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_create_size_length(const char *inData, size_t size, size_t length, U8String **outStr);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_create_sub_str(const U8String *inStr, const U8StrIter *start, const U8StrIter *end, U8String **outStr);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_create_clone(const U8String *inStr, U8String **outStr);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_init_ref(const char *inData, U8String *outRefStr);

U8S_DECL U8S_ERR_CODE U8S_CALL u8s_clear(U8String *str);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_delete(U8String *str);

U8S_DECL U8S_ERR_CODE U8S_CALL u8s_insert_str(U8String *str1, U8StrIter* iter, const U8String* str2);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_insert_cp(U8String *str, U8StrIter* iter, uchar32 cp);

/**
 * On succes end becomes invalid!
 * @param start
 * @param end
 * @return
 */
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_erase(U8String *str, U8StrIter* start, U8StrIter *end);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_erase_cp(U8String *str, U8StrIter* iter);

U8S_DECL U8S_ERR_CODE U8S_CALL u8s_replace(U8String *str1, U8StrIter* start, U8StrIter *end, const U8String* str2);

/******************************************************************************/
/*************************    ITERATORS FUNCTIONS    **************************/
/******************************************************************************/

U8S_DECL U8S_ERR_CODE U8S_CALL u8s_iter_init_begin(const U8String *str, U8StrIter *iter);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_iter_init_end(const U8String *str, U8StrIter *iter);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_iter_init_index(const U8String *str, size_t index, U8StrIter *iter);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_iter_advance(const U8String *str, U8StrIter *iter, int distance);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_iter_advance_fwd(const U8String *str, U8StrIter *iter, size_t distance);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_iter_advance_back(const U8String *str, U8StrIter *iter, size_t distance);
U8S_DECL U8S_ERR_CODE U8S_CALL u8s_iter_read_fwd(const U8String *str, U8StrIter *iter, uchar32 *cp);

/******************************************************************************/
/*************************    AUXILIARY FUNCTIONS    **************************/
/******************************************************************************/

/**
 * Calculate size in bytes of one UTF8 encoded code point.
 *
 * @param cpUTF8 UTF8 encoded code point.
 * @return .
 */
U8S_DECL U8S_ERR_CODE U8S_CALL u8sa_get_u8cp_size(const char *cpUTF8, size_t *size);

/**
 * Reads a single code point from UTF-8 sequence pointed by str.
 *
 * @param str UTF-8 string.
 * @param cp result code point.
 * @return .
 */
U8S_DECL U8S_ERR_CODE U8S_CALL u8sa_read_cp(const char *str, uchar32 *cp, size_t *bytesRead);

/**
 * Write a single code point to UTF-8 sequence pointed by str.
 *
 * @param str string.
 * @param cp code point to write.
 * @return .
 */
U8S_DECL U8S_ERR_CODE U8S_CALL u8sa_write_cp(char *dst, uchar32 cp, size_t *bytesWritten);

/**
 * Calculate bytes number need to store code point in UTF8.
 *
 * @param cp code point.
 * @return Size of code point encoded in UTF8 in bytes. 0 for illegal code point.
 */
U8S_DECL size_t U8S_CALL u8sa_get_cp_size(uchar32 cp);

/**
 * Calculate size(bytes) and length(code points) of UTF8 string.
 *
 * @param str UTF8 string.
 * @param size size of string in bytes. Without null terminator. Can be NULL if not needed.
 * @param length length of string in code points. Can be NULL if not needed.
 * @return .
 */
U8S_DECL U8S_ERR_CODE U8S_CALL u8sa_get_str_size_len(const char *str, size_t *size, size_t *length);

#ifdef __cplusplus
}
#endif

#endif /* IG_U8STRING_H_ */
