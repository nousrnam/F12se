/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

/*
TODO out of range iterators check
TODO all  memcpy and memmove check (after null terminator)
TODO verbs in description write or writes etc
*/

/* Possible skip check defines
U8S_SKIP_NULL_PTR_CHECK
U8S_SKIP_REF_CHECK
U8S_SKIP_RANGE_CHECK
U8S_SKIP_ITER_CHECK
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "U8String.h"

/* Code points > U8S_UNICODE_LIMIT are illegal */
#define U8S_UNICODE_LIMIT 0x10FFFF

/* x <= 01111111. ASCII  */
#define U8S_IS_SINGLE_BYTE(x) ((unsigned char)(x) <= 0x7F)
/* Valid lead byte must be at least 11000000(0xC0) */
#define U8S_IS_LEAD_BYTE(x) ((unsigned char)(x) >= 0xC0)
#define U8S_IS_FIRST_BYTE(x) ((U8S_IS_SINGLE_BYTE(x)) || (U8S_IS_LEAD_BYTE(x)))
/* Trail byte must be 10xxxxxx. (x & 11000000) == 10000000 */
#define U8S_IS_TRAIL_BYTE(x) (((unsigned char)(x) & 0xC0) == 0x80)

#define U8S_IS_STR_ASCII(x) ((x)->length + U8S_TERMINATOR_SIZE == (x)->memoryUsed)

#define U8S_MAX_CP_ONE_BYTE 0x7F
#define U8S_MAX_CP_TWO_BYTE 0x7FF
#define U8S_MAX_CP_THREE_BYTE 0xFFFF

#define U8S_1_BYTE_TO_CP(charArray, cp)     \
    ((cp) = charArray[0])
#define U8S_2_BYTE_TO_CP(charArray, cp)     \
    ((cp) = ((charArray[0] & 0x1F) << 6)    \
 +  (charArray[1] & 0x3F))
#define U8S_3_BYTE_TO_CP(charArray, cp)     \
    ((cp) = ((charArray[0] & 0x0F) << 12)   \
 + ((charArray[1] & 0x3F) << 6)             \
 +  (charArray[2] & 0x3F))
#define U8S_4_BYTE_TO_CP(charArray, cp)     \
    ((cp) = ((charArray[0] & 0x07) << 18)   \
 + ((charArray[1] & 0x3F) << 12)            \
 + ((charArray[2] & 0x3F) << 6)             \
 +  (charArray[3] & 0x3F))

#define U8S_CP_TO_1_BYTE(cp, charArray)         \
    charArray[0] = cp;
#define U8S_CP_TO_2_BYTE(cp, charArray)         \
    {                                           \
    charArray[0] = 0xC0 | ((cp >> 6) & 0x1F);   \
    charArray[1] = 0x80 | (cp & 0x3F);          \
    }
#define U8S_CP_TO_3_BYTE(cp, charArray)         \
    {                                           \
    charArray[0] = 0xE0 | ((cp >> 12) & 0x0F);  \
    charArray[1] = 0x80 | ((cp >>  6) & 0x3F);  \
    charArray[2] = 0x80 | (cp & 0x3F);          \
    }
#define U8S_CP_TO_4_BYTE(cp, charArray)         \
    {                                           \
    charArray[0] = 0xF0 | ((cp >> 18) & 0x07);  \
    charArray[1] = 0x80 | ((cp >> 12) & 0x3F);  \
    charArray[2] = 0x80 | ((cp >> 6) & 0x3F);   \
    charArray[3] = 0x80 | (cp & 0x3F);          \
    }


U8S_ERR_CODE u8s_iter_advance_fwd_(const U8String *str, U8StrIter *iter, size_t distance);
U8S_ERR_CODE u8sa_get_u8cp_size_(const char *cpUTF8, size_t *size);
U8S_ERR_CODE u8sa_read_cp_(const char *str, uchar32 *cp, size_t *bytesRead);


/******************************************************************************/
/******************************************************************************/
/***************************    PRIVATE FUNCTIONS    **************************/
/******************************************************************************/
/******************************************************************************/

/**
 * Check string validness.
 *
 * @param str string to check
 * @return 0 for invalid string, not 0 otherwise.
 */
short U8S_U8StrIsValid(const U8String* str)
    {
    size_t size;
    size_t length;

    if (str == NULL)
        return 0;

    if(str->data == NULL)
        return 0;

    if(str->memoryUsed < U8S_TERMINATOR_SIZE)
        return 0;

    if (u8sa_get_str_size_len(str->data, &size, &length) != U8S_ERROR_SUCCESS)
        return 0;

    if (str->length == length && str->memoryUsed == size + U8S_TERMINATOR_SIZE)
        return 1;

    return 0;
    }

/**
 * Check iterator validness.
 * @param str string to check
 * @return 0 for invalid string, not 0 otherwise.
 */
short U8S_U8IterIsValid(const U8String* str, const U8StrIter* iter)
    {
    size_t bytesRead;
    uchar32 curCodePoint;
    U8S_ERR_CODE errCode;

    if (iter == NULL || str == NULL || str->data == NULL)
        return 0;

    if(iter->offset >= str->memoryUsed)
        return 0;

    if(iter->position > str->length)
        return 0;

    errCode = u8sa_read_cp(&str->data[iter->offset], &curCodePoint, &bytesRead);
    if (errCode != U8S_ERROR_SUCCESS)
        return 0;

    return 1;
    }

/**
 * Common part for U8S_Create*** functions.
 *
 * @param inData
 * @param dataSize size of data without null terminator.
 * @param dataLength size of data in code points.
 * @param outStr
 * @return
 */
U8S_ERR_CODE u8s_create_common(const char *inData, size_t dataSize, size_t dataLength, U8String **outStr)
    {
    char *dataCopy;

    /* Try to allocate memory for new string */
    *outStr = (U8String*)malloc(sizeof(U8String));
    if (*outStr == NULL)
        return U8S_ERROR_MEM_ALLOC_FAIL;

    /* Try to allocate memory for copy of data */
    dataCopy = (char*)malloc(dataSize + U8S_TERMINATOR_SIZE);
    if (dataCopy == NULL)
        {
        free(*outStr);
        return U8S_ERROR_MEM_ALLOC_FAIL;
        }

    memcpy((void*)dataCopy, (const void*)inData, dataSize);
    dataCopy[dataSize] = 0; /* Null terminator */

    dataSize += U8S_TERMINATOR_SIZE;
    (**outStr).data = dataCopy;
    (**outStr).memoryAllocated = dataSize;
    (**outStr).memoryUsed = dataSize;
    (**outStr).length = dataLength;

    return U8S_ERROR_SUCCESS;
    }

/**
 * All checks must be passed before this function.
 */
U8S_ERR_CODE u8s_erase_common(U8String *str, size_t offset, size_t size, size_t length)
    {
    assert(size >= length);
    assert(str != NULL);

    /* NOTE for me, to remember:
     * str = "abcde"0
     * str->memoryUsed == 6
     * U8S_EraseCommonPart(str, 2, 1, 1)
     * str == "abde"0
     * memmoveSize == 3
     */

    memmove(str->data + offset, str->data + offset + size, str->memoryUsed - offset - size);
    str->memoryUsed -= size;
    str->length -= length;

    assert(U8S_U8StrIsValid(str));

    return U8S_ERROR_SUCCESS;
    }

/**
 * Prepares string for memory block insertion.
 *
 * U8String somestring = "foobar"
 * U8S_PrepareStringForInsert(&somestring, 3, 4)
 * somestring == "foo????bar"
 *
 * @param str
 * @param offset
 * @param size
 * @return
 */
U8S_ERR_CODE u8s_prepare_str_for_insert(U8String *str, size_t offset, size_t blockSize, size_t blockLen)
    {
    size_t needMemory;
    char *newData;

    if (blockSize == 0)
        return U8S_ERROR_SUCCESS;

    /* NOTE for me, to remember:
     * str = ""0
     * memoryUsed == 1
     * maximum insertion offset == 0
     * str = "abc"0
     * memoryUsed == 4
     * maximum insertion offset == 3
     * so "str->memoryUsed - offset" is at least U8S_TERMINATOR_SIZE.
     */

    needMemory = str->memoryUsed + blockSize;

    if (str->memoryAllocated < needMemory)
        {
        newData = (char*)malloc(needMemory);
        if (newData == NULL)
            return U8S_ERROR_MEM_ALLOC_FAIL;
        /* Copy memory before insertion offset. */
        memcpy(newData, str->data, offset);
        /* Copy memory after insertion offset. */
        memcpy(newData + offset + blockSize, str->data + offset, str->memoryUsed - offset);
        free(str->data);
        str->data = newData;
        str->memoryAllocated = needMemory;
        }
    else
        {
        memmove(str->data + offset + blockSize, str->data + offset, str->memoryUsed - offset);
        }
    str->memoryUsed = needMemory;
    str->length += blockLen;

    return U8S_ERROR_SUCCESS;
    }

/******************************************************************************/
/******************************************************************************/
/***************************    PUBLIC FUNCTIONS    ***************************/
/******************************************************************************/
/******************************************************************************/

U8S_ERR_CODE U8S_CALL u8s_create(const char *inData, U8String **outStr)
    {
    size_t length;
    size_t size;
    U8S_ERR_CODE errCode;

    assert(inData != NULL);

    errCode = u8sa_get_str_size_len(inData, &size, &length);
    if (errCode != U8S_ERROR_SUCCESS)
        return errCode;

    errCode = u8s_create_common(inData, size, length, outStr);

    assert(U8S_U8StrIsValid(*outStr));

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_create_size(const char *inData, size_t size, U8String **outStr)
    {
    size_t bytesRead;
    size_t length = 0;
    size_t curSize;
    const char *curData = inData;
    uchar32 curCodePoint;
    U8S_ERR_CODE errCode;

    assert(inData != NULL);

    while (curData < inData + size)
        {
        errCode = u8sa_read_cp(curData, &curCodePoint, &bytesRead);
        if (errCode != U8S_ERROR_SUCCESS)
            return errCode;

        if (curCodePoint == 0)
            break;

        curData += bytesRead;
        length++;
        }

    curSize = curData - inData;
    errCode = u8s_create_common(inData, curSize, length, outStr);

    assert(U8S_U8StrIsValid(*outStr));

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_create_length(const char *inData, size_t length, U8String **outStr)
    {
    size_t bytesRead;
    size_t curLength = 0;
    size_t curSize;
    const char *curData = inData;
    uchar32 curCodePoint;
    U8S_ERR_CODE errCode;

    assert(inData != NULL);

    while (curLength < length)
        {
        errCode = u8sa_read_cp_(curData, &curCodePoint, &bytesRead);
        if (errCode != U8S_ERROR_SUCCESS)
            return errCode;

        if (curCodePoint == 0)
            break;

        curData += bytesRead;
        curLength++;
        }

    curSize = curData - inData;
    errCode = u8s_create_common(inData, curSize, curLength, outStr);

    assert(U8S_U8StrIsValid(*outStr));

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_create_size_length(const char *inData, size_t size, size_t length, U8String **outStr)
    {
    U8S_ERR_CODE errCode;

    assert(inData != NULL);

    errCode = u8s_create_common(inData, size, length, outStr);

    assert(U8S_U8StrIsValid(*outStr));

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_create_sub_str(const U8String *inStr, const U8StrIter *start, const U8StrIter *end, U8String **outStr)
    {
    U8S_ERR_CODE errCode;
    size_t length;
    size_t size;

    assert(U8S_U8StrIsValid(inStr));
    assert(start != NULL);
    assert(end != NULL);

    #ifndef U8S_SKIP_RANGE_CHECK
    if (start->offset >= inStr->memoryUsed)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    assert(U8S_U8IterIsValid(inStr, start));
    assert(U8S_U8IterIsValid(inStr, end));

    if (end->offset > start->offset)
        {
        length = end->position - start->position;
        size = end->offset - start->offset;
        }
    else
        {
        length = 0;
        size = 0;
        }

    errCode = u8s_create_common(&inStr->data[start->offset], size, length, outStr);

    assert(U8S_U8StrIsValid(*outStr));

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_create_clone(const U8String *inStr, U8String **outStr)
    {
    U8S_ERR_CODE errCode;

    assert(U8S_U8StrIsValid(inStr));

    errCode = u8s_create_common(inStr->data, u8s_get_size(inStr), u8s_get_length(inStr), outStr);

    assert(U8S_U8StrIsValid(*outStr));

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_init_ref(const char *inData, U8String *outRefStr)
    {
    size_t length;
    size_t size;
    U8S_ERR_CODE errCode;

    assert(inData != NULL);
    assert(outRefStr != NULL);

    errCode = u8sa_get_str_size_len(inData, &size, &length);
    if (errCode != U8S_ERROR_SUCCESS)
        return errCode;

    outRefStr->data = (char*)inData;
    outRefStr->length = length;
    outRefStr->memoryUsed = size + U8S_TERMINATOR_SIZE;
    outRefStr->memoryAllocated = 0;

    assert(U8S_U8StrIsValid(outRefStr));

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_clear(U8String *str)
    {
    assert(str != NULL);

    #ifndef U8S_SKIP_REF_CHECK
    if (str->memoryAllocated == 0)
        return U8S_ERROR_REFERENCE_MOD;
    #endif

    assert(U8S_U8StrIsValid(str));

    str->memoryUsed = U8S_TERMINATOR_SIZE;
    str->length = 0;
    str->data[0] = 0;
    return U8S_ERROR_SUCCESS;
    }


U8S_ERR_CODE U8S_Delete_(U8String *str)
    {
    free(str->data);
    free(str);
    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8s_delete(U8String *str)
    {
    assert(U8S_U8StrIsValid(str));

    #ifndef U8S_SKIP_REF_CHECK
    if (str->memoryAllocated == 0)
        return U8S_ERROR_REFERENCE_MOD;
    #endif

    return U8S_Delete_(str);
    }

U8S_ERR_CODE U8S_CALL u8s_insert_str(U8String *str1, U8StrIter* iter, const U8String* str2)
    {
    U8S_ERR_CODE errCode;
    size_t str2Size;
    U8String* cloneStr;

    assert(U8S_U8StrIsValid(str1));
    assert(U8S_U8StrIsValid(str2));
    assert(U8S_U8IterIsValid(str1, iter));

    #ifndef U8S_SKIP_RANGE_CHECK
    if (iter->offset >= str1->memoryUsed)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    /* Check self insertion */
    if (str1 == str2)
        {
        errCode = u8s_create_clone(str1, &cloneStr);
        if (errCode != U8S_ERROR_SUCCESS)
            return errCode;
        /* Insert clone */
        errCode = u8s_insert_str(str1, iter, cloneStr);
        U8S_Delete_(cloneStr);
        return errCode;
        }

    str2Size = u8s_get_size(str2);

    errCode = u8s_prepare_str_for_insert(str1, iter->offset, str2Size, str2->length);
    if (errCode != U8S_ERROR_SUCCESS)
        return errCode;

    memcpy(&str1->data[iter->offset], str2->data, str2Size);

    assert(U8S_U8StrIsValid(str1));

    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8s_insert_cp(U8String *str, U8StrIter* iter, uchar32 cp)
    {
    U8S_ERR_CODE errCode;
    size_t cpSize;

    assert(U8S_U8StrIsValid(str));
    assert(U8S_U8IterIsValid(str, iter));

    #ifndef U8S_SKIP_RANGE_CHECK
    if (iter->offset >= str->memoryUsed)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    cpSize = u8sa_get_cp_size(cp);
    if (cpSize == 0)
        return U8S_ERROR_ABOVE_UNICODE_LIMIT;

    errCode = u8s_prepare_str_for_insert(str, iter->offset, cpSize, 1);
    if (errCode != U8S_ERROR_SUCCESS)
        return errCode;

    errCode = u8sa_write_cp(&str->data[iter->offset], cp, &cpSize);

    assert(U8S_U8StrIsValid(str));

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_erase(U8String *str, U8StrIter* start, U8StrIter *end)
    {
    assert(U8S_U8StrIsValid(str));
    assert(U8S_U8IterIsValid(str, start));
    assert(U8S_U8IterIsValid(str, end));

    #ifndef U8S_SKIP_RANGE_CHECK
    if (start->offset >= str->memoryUsed || end->offset >= str->memoryUsed)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    #ifndef U8S_SKIP_REF_CHECK
    if (str->memoryAllocated == 0)
        return U8S_ERROR_REFERENCE_MOD;
    #endif

    if (start->offset >= end->offset)
        return U8S_ERROR_SUCCESS;

    return u8s_erase_common(str, start->offset, end->offset - start->offset, end->position - start->position);
    }

U8S_ERR_CODE U8S_CALL u8s_erase_cp(U8String *str, U8StrIter* iter)
    {
    U8S_ERR_CODE errCode;
    size_t cpSize;

    assert(U8S_U8StrIsValid(str));
    assert(U8S_U8IterIsValid(str, iter));

    #ifndef U8S_SKIP_REF_CHECK
    if (str->memoryAllocated == 0)
        return U8S_ERROR_REFERENCE_MOD;
    #endif

    #ifndef U8S_SKIP_RANGE_CHECK
    if (iter->offset + U8S_TERMINATOR_SIZE >= str->memoryUsed)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    errCode = u8sa_get_u8cp_size(&str->data[iter->offset], &cpSize);
    if (errCode != U8S_ERROR_SUCCESS)
        return errCode;

    return u8s_erase_common(str, iter->offset, cpSize, 1);
    }


U8S_ERR_CODE U8S_Replace_(U8String* inStr, U8StrIter* start, U8StrIter *end, const U8String* replStr)
    {
    size_t changeSize = end->offset - start->offset;
    size_t replSize = u8s_get_size(replStr);
    char *newData;
    size_t needMemory = u8s_get_size(inStr) - changeSize + replSize;


    /* Check if sizes match -> no need move or realloc memory. */
    if (changeSize == replSize)
        {
        ;
        }
    /* TODO test this variant */
    else if (inStr->memoryAllocated >= needMemory)
        {
        /* inStr->memoryUsed - end->offset is at least U8S_TERMINATOR_SIZE */
        memmove(inStr->data + start->offset + replSize, inStr->data + start->offset + changeSize, inStr->memoryUsed - end->offset);
        }
    else
        {
        /* Need more memory */
        newData = (char*)malloc(needMemory);
        if (newData == NULL)
            return U8S_ERROR_MEM_ALLOC_FAIL;
        /* Copy memory before insertion offset. */
        memcpy(newData, inStr->data, start->offset);
        /* Copy memory after insertion offset. */
        memcpy(newData + start->offset + replSize, inStr->data + start->offset + changeSize, inStr->memoryUsed - end->offset);
        free(inStr->data);
        inStr->data = newData;
        inStr->memoryAllocated = needMemory;
        }

    inStr->memoryUsed = needMemory;
    inStr->length = inStr->length + replStr->length - (end->position - start->position);

    memcpy(inStr->data + start->offset, replStr->data, replSize);

    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8s_replace(U8String *str1, U8StrIter* start, U8StrIter *end, const U8String* str2)
    {
    U8S_ERR_CODE errCode;

    assert(U8S_U8StrIsValid(str1));
    assert(U8S_U8StrIsValid(str2));
    assert(U8S_U8IterIsValid(str1, start));
    assert(U8S_U8IterIsValid(str1, end));

    #ifndef U8S_SKIP_RANGE_CHECK
    if (start->offset >= str1->memoryUsed)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    errCode = U8S_Replace_(str1, start, end, str2);

    assert(U8S_U8StrIsValid(str1));

    return errCode;
    }

/******************************************************************************/
/*************************    ITERATORS FUNCTIONS    **************************/
/******************************************************************************/


U8S_ERR_CODE U8S_CALL u8s_iter_init_begin(const U8String *str, U8StrIter *iter)
    {
    (void)(str);
    assert(U8S_U8StrIsValid(str));
    assert(iter != NULL);

    iter->offset = 0;
    iter->position = 0;

    assert(U8S_U8IterIsValid(str, iter));

    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8s_iter_init_end(const U8String *str, U8StrIter *iter)
    {
    assert(U8S_U8StrIsValid(str));
    assert(iter != NULL);

    iter->offset = u8s_get_size(str);
    iter->position = u8s_get_length(str);

    assert(U8S_U8IterIsValid(str, iter));

    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8s_iter_init_index(const U8String *str, size_t index, U8StrIter *iter)
    {
    U8S_ERR_CODE errCode;

    assert(U8S_U8StrIsValid(str));
    assert(iter != NULL);

    #ifndef U8S_SKIP_RANGE_CHECK
    if (index > str->length)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    if (U8S_IS_STR_ASCII(str))
        {
        iter->offset = index;
        iter->position = index;
        return U8S_ERROR_SUCCESS;
        }

    if (index < (str->length / 2))
        {
        iter->offset = 0;
        iter->position = 0;
        errCode = u8s_iter_advance_fwd_(str, iter, index);
        }
    else
        {
        iter->offset = u8s_get_size(str);
        iter->position = u8s_get_length(str);
        errCode = u8s_iter_advance_back(str, iter, u8s_get_length(str) - index);
        }

    return errCode;
    }

U8S_ERR_CODE U8S_CALL u8s_iter_advance(const U8String *str, U8StrIter *iter, int distance)
    {
    assert(U8S_U8StrIsValid(str));
    assert(U8S_U8IterIsValid(str, iter));

    if (distance >= 0)
        return u8s_iter_advance_fwd(str, iter, distance);
    else
        return u8s_iter_advance_back(str, iter, -distance);
    }

U8S_ERR_CODE u8s_iter_advance_fwd_(const U8String *str, U8StrIter *iter, size_t distance)
    {
    U8S_ERR_CODE errCode;
    uchar32 cp;
    size_t bytesRead;

    iter->position += distance;
    while (distance > 0)
        {
        errCode = u8sa_read_cp_(&str->data[iter->offset], &cp, &bytesRead);
        if (errCode != U8S_ERROR_SUCCESS)
            return errCode;
        iter->offset += bytesRead;
        distance--;
        }

    assert(U8S_U8IterIsValid(str, iter));

    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8s_iter_advance_fwd(const U8String *str, U8StrIter *iter, size_t distance)
    {
    assert(U8S_U8StrIsValid(str));
    assert(U8S_U8IterIsValid(str, iter));

    #ifndef U8S_SKIP_RANGE_CHECK
    if (iter->position + distance > str->length)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    if (U8S_IS_STR_ASCII(str))
        {
        iter->offset += distance;
        iter->position += distance;
        return U8S_ERROR_SUCCESS;
        }

    return u8s_iter_advance_fwd_(str, iter, distance);
    }

U8S_ERR_CODE U8S_CALL u8s_iter_advance_back(const U8String *str, U8StrIter *iter, size_t distance)
    {
    unsigned char *beginChar;
    unsigned char *curChar;
    uchar32 cp = 0;

    size_t trailNum = 0;

    /* TODO Something. This function is big and complicated. */
    /* Maybet 3 functions: getback(�������� ������ �����), readback(������������� �� ������ �����), advanceback(������������� �� ����� ��������) */

    assert(U8S_U8StrIsValid(str));
    assert(U8S_U8IterIsValid(str, iter));

    #ifndef U8S_SKIP_RANGE_CHECK
    if (iter->position < distance)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    if (U8S_IS_STR_ASCII(str))
        {
        iter->offset -= distance;
        iter->position -= distance;
        return U8S_ERROR_SUCCESS;
        }

    beginChar = (unsigned char*)str->data;
    curChar = &beginChar[iter->offset];

    iter->position -= distance;
    while (curChar > beginChar && distance > 0)
        {
        curChar--;

        /* ASCII character */
        if (U8S_IS_SINGLE_BYTE(*curChar))
            {
            distance--;
            continue;
            }

        if (U8S_IS_TRAIL_BYTE(*curChar))
            {
            cp = cp | ((*curChar & 0x3F) << (6 * trailNum)); /* TRAIL_BYTE = 10xxxxxx. 8-2 == 6 */

            trailNum++;
            if (trailNum >= U8S_CP_MAX_SIZE_BYTE)
                return U8S_ERROR_NOT_UTF8;

            continue;
            }

        if (U8S_IS_LEAD_BYTE(*curChar))
            {
            distance--;

            switch (trailNum)
                {
                case 1:
                    cp = cp | ((*curChar & 0x1F) << (6 * trailNum));
                    break;
                case 2:
                    cp = cp | ((*curChar & 0x0F) << (6 * trailNum));
                    break;
                case 3:
                    cp = cp | ((*curChar & 0x07) << (6 * trailNum));
                    break;
                default:
                    return U8S_ERROR_UNKNOWN;
                }
            if (cp > U8S_UNICODE_LIMIT)
                return U8S_ERROR_ABOVE_UNICODE_LIMIT;
            cp = 0;
            trailNum = 0;
            continue;
            }

        return U8S_ERROR_NOT_UTF8;
        }

    iter->offset = curChar - beginChar;

    assert(U8S_U8IterIsValid(str, iter));

    /* TODO return not only success */

    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8s_iter_read_fwd(const U8String *str, U8StrIter *iter, uchar32 *cp)
    {
    U8S_ERR_CODE errCode;
    size_t bytesRead;

    assert(U8S_U8StrIsValid(str));
    assert(U8S_U8IterIsValid(str, iter));

    #ifndef U8S_SKIP_RANGE_CHECK
    if (iter->position >= str->length)
        return U8S_ERROR_OUT_OF_RANGE;
    #endif

    errCode = u8sa_read_cp_(&str->data[iter->offset], cp, &bytesRead);
    if (errCode != U8S_ERROR_SUCCESS)
        return errCode;
    iter->offset += bytesRead;
    iter->position += 1;

    assert(U8S_U8IterIsValid(str, iter));

    return U8S_ERROR_SUCCESS;
    }

/******************************************************************************/
/*************************    AUXILIARY FUNCTIONS    **************************/
/******************************************************************************/

U8S_ERR_CODE U8S_CALL u8sa_get_u8cp_size_(const char *cpUTF8, size_t *size)
    {
    unsigned char c;
    #ifndef U8S_SKIP_FULL_CHECK
    uchar32 cp;
    #endif

    if (U8S_IS_SINGLE_BYTE(cpUTF8[0]))
        {
        *size = 1;  /* ASCII character */
        return U8S_ERROR_SUCCESS;
        }

    c = cpUTF8[0];

    /* It is not ASCII, so byte must be lead */
    if (!U8S_IS_LEAD_BYTE(c))
        {
        *size = 0;
        return U8S_ERROR_NOT_UTF8;
        }

    /* 2 bytes UTF8 */
    if (c < 0xE0) /* 11100000 */
        {
        /* Check second byte. must be 10xxxxxx */
        if (U8S_IS_TRAIL_BYTE(cpUTF8[1]))
            {
            *size = 2;
            return U8S_ERROR_SUCCESS;
            }
        /* cpUTF8 is invalid utf8 */
        return U8S_ERROR_NOT_UTF8;
        }

    /* 3 bytes UTF8 */
    if (c < 0xF0) /* 11110000 */
        {
        /* Check 2 and 3 byte. must be 10xxxxxx */
        if (U8S_IS_TRAIL_BYTE(cpUTF8[1]) && U8S_IS_TRAIL_BYTE(cpUTF8[2]))
            {
            *size = 3;
            return U8S_ERROR_SUCCESS;
            }
        /* cpUTF8 is invalid utf8 */
        return U8S_ERROR_NOT_UTF8;
        }

    if (U8S_IS_TRAIL_BYTE(cpUTF8[1])
        && U8S_IS_TRAIL_BYTE(cpUTF8[2])
        && U8S_IS_TRAIL_BYTE(cpUTF8[3]))
        {
        #ifndef U8S_SKIP_FULL_CHECK
        U8S_4_BYTE_TO_CP(cpUTF8, cp);
        if (cp > U8S_UNICODE_LIMIT)
            {
            *size = 0;
            return U8S_ERROR_ABOVE_UNICODE_LIMIT;
            }
        #endif

        *size = 4;
        return U8S_ERROR_SUCCESS;
        }

    *size = 0;
    return U8S_ERROR_NOT_UTF8;
    }

U8S_ERR_CODE U8S_CALL u8sa_get_u8cp_size(const char *cpUTF8, size_t *size)
    {
    #ifndef U8S_SKIP_FULL_CHECK
    if (cpUTF8 == NULL || size == NULL)
        return U8S_ERROR_NULL_PTR;
    #endif

    return u8sa_get_u8cp_size_(cpUTF8, size);
    }

U8S_ERR_CODE u8sa_read_cp_(const char *str, uchar32 *cp, size_t *bytesRead)
    {
    size_t charSize;
    U8S_ERR_CODE errCode;

    errCode = u8sa_get_u8cp_size_(str, &charSize);
    if (errCode != U8S_ERROR_SUCCESS)
        return errCode;

    switch (charSize)
        {
        case 1:
            U8S_1_BYTE_TO_CP(str, *cp);
            break;
        case 2:
            U8S_2_BYTE_TO_CP(str, *cp);
            break;
        case 3:
            U8S_3_BYTE_TO_CP(str, *cp);
            break;
        case 4:
            U8S_4_BYTE_TO_CP(str, *cp);
            break;
        default:
            return U8S_ERROR_UNKNOWN;
            break;
        }

    *bytesRead = charSize;

    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8sa_read_cp(const char *str, uchar32 *cp, size_t *bytesRead)
    {
    size_t charSize;
    uchar32 c;
    U8S_ERR_CODE errCode;

    #ifndef U8S_SKIP_NULL_PTR_CHECK
    if (str == NULL || cp == NULL)
        return U8S_ERROR_NULL_PTR;
    #endif

    errCode = u8sa_read_cp_(str, &c, &charSize);
    if (errCode != U8S_ERROR_SUCCESS)
        return errCode;

    if (cp != NULL)
        *cp = c;

    if (bytesRead != NULL)
        *bytesRead = charSize;

    return U8S_ERROR_SUCCESS;
    }

U8S_ERR_CODE U8S_CALL u8sa_write_cp(char *dst, uchar32 cp, size_t *bytesWritten)
    {
    #ifndef U8S_SKIP_NULL_PTR_CHECK
    if (dst == NULL)
        return U8S_ERROR_NULL_PTR;
    #endif

    if (cp <= U8S_MAX_CP_ONE_BYTE)
        {
        U8S_CP_TO_1_BYTE(cp, dst);
        if (bytesWritten != NULL)
            *bytesWritten = 1;
        return U8S_ERROR_SUCCESS;
        }

    if (cp <= U8S_MAX_CP_TWO_BYTE)
        {
        U8S_CP_TO_2_BYTE(cp, dst);
        if (bytesWritten != NULL)
            *bytesWritten = 2;
        return U8S_ERROR_SUCCESS;
        }

    if (cp <= U8S_MAX_CP_THREE_BYTE)
        {
        U8S_CP_TO_3_BYTE(cp, dst);
        if (bytesWritten != NULL)
            *bytesWritten = 3;
        return U8S_ERROR_SUCCESS;
        }

    if (cp <= U8S_UNICODE_LIMIT)
        {
        U8S_CP_TO_4_BYTE(cp, dst);
        if (bytesWritten != NULL)
            *bytesWritten = 4;
        return U8S_ERROR_SUCCESS;
        }

    return U8S_ERROR_ABOVE_UNICODE_LIMIT;
    }

size_t U8S_CALL u8sa_get_cp_size(uchar32 cp)
    {
    if (cp <= U8S_MAX_CP_ONE_BYTE)
       return 1;
    if (cp <= U8S_MAX_CP_TWO_BYTE)
       return 2;
    if (cp <= U8S_MAX_CP_THREE_BYTE)
       return 3;
    if (cp <= U8S_UNICODE_LIMIT)
       return 4;

    return 0; /* Illegal code point. == U8S_ERROR_ABOVE_UNICODE_LIMIT */
    }

U8S_ERR_CODE U8S_CALL u8sa_get_str_size_len(const char *str, size_t* size, size_t* length)
    {
    size_t curCharSize;
    size_t tmpLength = 0;
    const char *curStr;
    U8S_ERR_CODE errCode = U8S_ERROR_SUCCESS;

    #ifndef U8S_SKIP_NULL_PTR_CHECK
    if (str == NULL)
        return U8S_ERROR_NULL_PTR;
    #endif

    curStr = str;
    while (*curStr != 0)
        {
        errCode = u8sa_get_u8cp_size(curStr, &curCharSize);
        if (errCode != U8S_ERROR_SUCCESS)
            {
            break;
            }
        tmpLength++;
        curStr += curCharSize;
        }

    if (size != NULL)
        *size = curStr - str;
    if (length != NULL)
        *length = tmpLength;

    return errCode;
    }

