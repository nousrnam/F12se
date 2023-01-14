#include <assert.h>
#include <errno.h>
#include <stdio.h> /* snprintf */
#include <limits.h>
#include <stdlib.h> /* strtol... */
#include <ctype.h> /* isspace */

#include "ntst_string.h"
#include "ntst_mem.h"
#include "ntst_bytes.h"

#if ('a'+1 == 'b') && ('b'+1 == 'c') && ('c'+1 == 'd') && ('d'+1 == 'e') && ('e'+1 == 'f') \
    && ('A'+1 == 'B') && ('B'+1 == 'C') && ('C'+1 == 'D') && ('D'+1 == 'E') && ('E'+1 == 'F') \
    && ('a' == 97) && ('A' == 65)
#define NTST_ASCII_ENCODING 1
#endif

int ntst_ascii_tolower(int ch)
    {
    if ((ch >= 'A') && (ch <= 'Z'))
        return ch | 32;
    return ch;
    }

int ntst_hex_char_to_int(char c)
    {
    if (NTST_ISDIGIT(c))
        return c - '0';
    // Для 'A' и 'Z' нет гарантий порядка, например EBCDIC
#if defined(NTST_ASCII_ENCODING)
    // TODO может сделать так?
    // c |= 32;
    // if ((c <= 'f') && (c >= 'a'))
    //     return c - 'a' + 10;
    /* В порядке расстановки условий есть логика. <='F' немногие нужные, а >='A' очень многие ненужные
       Дальше уже нет. Не могу прикинуть что в реале чаще <='f' или >='a'
       Надо бы в gcc.godbolt.org проверить все варианты, когда делать будет нечего. */
    if ((c <= 'F') && (c >= 'A'))
        return c - 'A' + 10;
    if ((c <= 'f') && (c >= 'a'))
        return c - 'a' + 10;
    /* Попытка уменьшить максимальное число сравнений, но тогда и от 0 до 9 тоже надо сюда затащить.
    if (c <= 'F')
        {
        if (c >= 'A')
            return c - 'A' + 10;
        }
    else if (c <= 'f')
        {
        if (c >= 'a')
            return c - 'a' + 10;
        }*/
#else
    switch(c)
        {
        case 'a': return 10;
        case 'b': return 11;
        case 'c': return 12;
        case 'd': return 13;
        case 'e': return 14;
        case 'f': return 15;
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        }
#endif
    return -1;
    }

int ntst_hex_char_pair_to_int(char c_high, char c_low)
    {
    int ret, l, h = ntst_hex_char_to_int(c_high);
    if (NTST_UNLIKELY(h < 0))
        return -1;
    l = ntst_hex_char_to_int(c_low);
    if (NTST_UNLIKELY(l < 0))
        return -2;
    ret = (h << 4) | l;
    return ret;
    }

static const char hex_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void ntst_byte_to2char(unsigned char byte, char* out)
    {
    *out = hex_chars[(byte>>4) & 0xf];
    ++out;
    *out =  hex_chars[byte & 0xf];
    }

int ntst_strhasprefix(const char* NTST_RESTRICT string, const char* NTST_RESTRICT prefix)
    {
    assert(string != NULL);
    assert(prefix != NULL);
    /* OR strncmp(string, prefix, strlen(prefix)) == 0; */
    while (*prefix != '\0')
        {
        if (*prefix != *string)
            return 0;
        ++prefix;
        ++string;
        }
    return 1;
    }

int ntst_snprintf(char* NTST_RESTRICT buff, unsigned max_len, const char* NTST_RESTRICT format, ...)
    {
    int result;
    va_list args;
    va_start(args, format);
    result = ntst_vsnprintf(buff, max_len, format, args);
    va_end(args);
    return result;
    }

int ntst_vsnprintf(char* NTST_RESTRICT buff, unsigned max_len, const char* NTST_RESTRICT format, va_list vlist)
    {
    int result;
    /* vsnprintf in MinGW is broken when uses Microsoft implementation.
       Returns -1 if buffer too small instead of required size. */
#if ((defined(__STDC__) && (__STDC_VERSION__ >= 199901L)) || defined(__GNUC__)) && !defined(__MINGW32__)
    /* По стандарту вроде при успехе возвращает число символов(без терминатора), которое было бы записано. */
    result = vsnprintf(buff, max_len, format, vlist);
    return result;
#elif defined(_MSC_VER) || defined(__MINGW32__)
    /* old msvc doesn't have va_copy. Only since Visual Studio 2013 */
    va_list args2;
#if (defined(_MSC_VER) && _MSC_VER < 1800) /* 12.0 (2013) */
    args2 = vlist;
    /* too bad cant use #warning */
#else
    va_copy(args2, vlist);
#endif
    result = vsnprintf(buff, max_len, format, vlist);
    /* У микрософта крайне неадекватные правила по нуль терминаторам. Воткнем сами на всякий случай. */
    if (max_len > 0)
        buff[max_len - 1] = '\0';
    if (result < 0)
        {
        result = _vscprintf(format, args2); /* Возвращаем длину */
        }
    va_end(args2);
    return result;
#else
#error Bad platform and compiler
#endif
    }

static void ntst_shift_bytes_left(char* buf, size_t n)
    {
    while (n != 0)
        {
        char* next = buf + 1;
        *buf = *next;
        buf = next;
        --n;
        }
    }

unsigned ntst_print_uint_dec_unsafe(char* buff, unsigned val)
    {
    char* cur = buff;
    do
        {
        const char c = '0' + (val % 10);
        val /= 10;
        *cur = c;
        ++cur;
        } while (val != 0);
    ntst_reverse_bytes_it(buff, cur);
    return cur - buff;
    }

unsigned ntst_print_uint_decz_unsafe(char* buff, unsigned val)
    {
    const unsigned count = ntst_print_uint_dec_unsafe(buff, val);
    buff[count] = '\0';
    return count + 1;
    }

unsigned ntst_print_int_dec_unsafe(char* buff, int val)
    {
    if (val < 0)
        {
        *buff = '-';
        ++buff;
        return 1 + ntst_print_uint_dec_unsafe(buff, -val);
        }
    return ntst_print_uint_dec_unsafe(buff, val);
    }

unsigned ntst_print_int_decz_unsafe(char* buff, int val)
    {
    const unsigned count = ntst_print_int_dec_unsafe(buff, val);
    buff[count] = '\0';
    return count + 1;
    }

unsigned ntst_print_uint_dec(char* buff, unsigned max_len, unsigned val)
    {
    char* cur;
    unsigned len;
    if (NTST_LIKELY(max_len >= NTST_PRINT_UNSIGNED_SIZE(val)-1))
        return ntst_print_uint_dec_unsafe(buff, val);
    if (NTST_UNLIKELY(max_len == 0))
        return 0;
    cur = buff;
    len = max_len;
    do
        {
        const char c = '0' + (val % 10);
        val /= 10;
        if (NTST_LIKELY(len != 0))
            {
            *cur = c;
            --len;
            ++cur;
            }
        else
            {
            const unsigned last_idx = max_len - 1;
            ntst_shift_bytes_left(buff, last_idx);
            buff[last_idx] = c;
            }
        } while (val != 0);
    ntst_reverse_bytes_it(buff, cur);
    return max_len - len;
    }

unsigned ntst_print_uint_decz(char* buff, unsigned max_len, unsigned val)
    {
    if (NTST_LIKELY(max_len >= NTST_PRINT_UNSIGNED_SIZE(val)))
        return ntst_print_uint_decz_unsafe(buff, val);
    if (NTST_LIKELY(max_len != 0))
        {
        const unsigned count = ntst_print_uint_dec(buff, max_len - 1, val);
        buff[count] = '\0';
        return count + 1;
        }
    return 0;
    }

unsigned ntst_print_int_dec(char* buff, unsigned max_len, int val)
    {
    if (NTST_LIKELY(max_len >= NTST_PRINT_SIGNED_SIZE(val)-1))
        return ntst_print_int_dec_unsafe(buff, val);
    if (val < 0)
        {
        if (NTST_LIKELY(max_len != 0))
            {
            *buff = '-';
            ++buff;
            --max_len;
            return 1 + ntst_print_uint_dec(buff, max_len, -val);
            }
        return 0;
        }
    return ntst_print_uint_dec(buff, max_len, val);
    }

unsigned ntst_print_int_decz(char* buff, unsigned max_len, int val)
    {
    if (NTST_LIKELY(max_len >= NTST_PRINT_SIGNED_SIZE(val)))
        return ntst_print_int_decz_unsafe(buff, val);
    if (NTST_LIKELY(max_len != 0))
        {
        const unsigned count = ntst_print_int_dec(buff, max_len - 1, val);
        buff[count] = '\0';
        return count + 1;
        }
    return 0;
    }

static unsigned ntst_do_pad_unsafe(char* buff, unsigned min_len, unsigned len, char pad)
    {
    const int shift = min_len - len;
    if (shift > 0)
        {
        int i;
        for (i = len; i > 0;)
            {
            const int nexti = i - 1;
            buff[nexti + shift] = buff[nexti];
            i = nexti;
            }
        for (i = 0; i < shift; ++i)
            buff[i] = pad;
        return min_len;
        }
    return len;
    }

unsigned ntst_print_pad_uint_dec_unsafe(char* buff, unsigned min_len, unsigned val, char pad)
    {
    const unsigned len = ntst_print_uint_dec_unsafe(buff, val);
    return ntst_do_pad_unsafe(buff, min_len, len, pad);
    }

unsigned ntst_print_pad_uint_decz_unsafe(char* buff, unsigned min_len, unsigned val, char pad)
    {
    const unsigned len = ntst_print_uint_decz_unsafe(buff, val);
    return ntst_do_pad_unsafe(buff, min_len, len, pad);
    }

unsigned ntst_print_pad_int_dec_unsafe(char* buff, unsigned min_len, int val, char pad)
    {
    const unsigned len = ntst_print_int_dec_unsafe(buff, val);
    return ntst_do_pad_unsafe(buff, min_len, len, pad);
    }

unsigned ntst_print_pad_int_decz_unsafe(char* buff, unsigned min_len, int val, char pad)
    {
    const unsigned len = ntst_print_int_decz_unsafe(buff, val);
    return ntst_do_pad_unsafe(buff, min_len, len, pad);
    }

static unsigned ntst_do_pad_safe(char* buff, unsigned min_len, unsigned max_len, unsigned len, char pad)
    {
    const int maxshift = max_len - len;
    int shift = min_len - len;
    if (shift > maxshift)
        shift = maxshift;
    if (shift > 0)
        {
        int i;
        for (i = len - 1; i > 0; --i)
            buff[i] = buff[i - shift];
        for (i = 0; i < shift; ++i)
            buff[i] = pad;
        return min_len;
        }
    return len;
    }

unsigned ntst_print_pad_uint_dec(char* buff, unsigned min_len, unsigned max_len, unsigned val, char pad)
    {
    const unsigned len = ntst_print_uint_dec(buff, max_len, val);
    return ntst_do_pad_safe(buff, min_len, max_len, len, pad);
    }

unsigned ntst_print_pad_uint_decz(char* buff, unsigned min_len, unsigned max_len, unsigned val, char pad)
    {
    const unsigned len = ntst_print_uint_decz(buff, max_len, val);
    return ntst_do_pad_safe(buff, min_len, max_len, len, pad);
    }

unsigned ntst_print_pad_int_dec(char* buff, unsigned min_len, unsigned max_len, int val, char pad)
    {
    const unsigned len = ntst_print_int_dec(buff, max_len, val);
    return ntst_do_pad_safe(buff, min_len, max_len, len, pad);
    }

unsigned ntst_print_pad_int_decz(char* buff, unsigned min_len, unsigned max_len, int val, char pad)
    {
    const unsigned len = ntst_print_int_decz(buff, max_len, val);
    return ntst_do_pad_safe(buff, min_len, max_len, len, pad);
    }

size_t ntst_str_concat(char* NTST_RESTRICT buff, size_t max_len, ...)
    {
    size_t result = 0;
    va_list args;
    va_start(args, max_len);
    for (;;)
        {
        const char* cur_str = va_arg(args, const char*);
        if (cur_str == NULL)
            break;
        while (*cur_str != '\0')
            {
            if (result < max_len)
                buff[result] = *cur_str;
            ++result;
            ++cur_str;
            }
        }
    va_end(args);
    return result;
    }

size_t ntst_strncpy(char* NTST_RESTRICT dest, const char* NTST_RESTRICT src, size_t count)
    {
    size_t i, count_minus_1;
    assert(dest != NULL);
    assert(src != NULL);
    if (NTST_UNLIKELY(count == 0))
        return 0;
    count_minus_1 = count - 1;
    for (i = 0; i < count_minus_1; ++i)
        {
        *dest = *src;
        if (NTST_UNLIKELY(*src == '\0'))
            {
            return i;
            }
        ++dest;
        ++src;
        }
    *dest = '\0';
    return count_minus_1;
    }

size_t ntst_str10toi(const char* NTST_RESTRICT str, int* NTST_RESTRICT result)
    {
    char *str_end;
    long res;
    errno = 0;
    res = strtol(str, &str_end, 10);
    if (NTST_UNLIKELY((str_end == str) || (errno != 0)))
        return 0;
#if (INT_MAX != LONG_MAX) || (INT_MIN != LONG_MIN)
    if (NTST_UNLIKELY((res > INT_MAX) || (res < INT_MIN)))
        return 0;
#endif
    *result = (int)res;
    return (str_end - str);
    }

size_t ntst_str10tol(const char* NTST_RESTRICT str, long* NTST_RESTRICT result)
    {
    char *str_end;
    long res;
    errno = 0;
    res = strtol(str, &str_end, 10);
    if (NTST_UNLIKELY((str_end == str) || (errno != 0)))
        return 0;
    *result = res;
    return (str_end - str);
    }

#if (defined(__STDC__) && (__STDC_VERSION__ >= 199901L)) || defined(__GNUC__)
#   define STRTOLL strtoll
#elif defined(_MSC_VER)
#   if (_MSC_VER >= 1800) /* 12.0 (2013) */
#       define STRTOLL strtoll
#   else
#       define STRTOLL _strtoi64
#   endif
#endif

size_t ntst_str10toll(const char* NTST_RESTRICT str, long long* NTST_RESTRICT result)
    {
    char *str_end;
    long long res;
    errno = 0;
    res = STRTOLL(str, &str_end, 10);
    if (NTST_UNLIKELY((str_end == str) || (errno != 0)))
        return 0;
    *result = res;
    return (str_end - str);
    }

size_t ntst_str10tou(const char* NTST_RESTRICT str, unsigned* NTST_RESTRICT result)
    {
    /* Под виндой strtoul Прекрасно схавает отрицательное значение не выставив ошибки.
     В гну мануале: If string depicts a negative number, strtoul acts the same as strtol but casts the result to an unsigned integer.
     На минусе никто кроме нас не обломается*/
    const char* str_ = str;
    char *str_end;
    unsigned long res;
    assert(str != NULL);
    assert(result != NULL);
    while (NTST_ISSPACE(*str))
        ++str;
    if (NTST_UNLIKELY(*str == '-'))
        return 0;
    errno = 0;
    res = strtoul(str, &str_end, 10);
    if (NTST_UNLIKELY((str_end == str) || (errno != 0)))
        return 0;
#if (UINT_MAX != ULONG_MAX)
    if (NTST_UNLIKELY(res > UINT_MAX))
        return 0;
#endif
    *result = (unsigned)res;
    return (str_end - str_);
    }

size_t ntst_strn10toi(const char* NTST_RESTRICT str, size_t len, int* NTST_RESTRICT result)
    {
    long val;
    const size_t ret = ntst_strn10tol(str, len, &val);
    /* (ret == 0) check is unnecessary. But valgrind does not like when val is not initialised.*/
#if (INT_MAX != LONG_MAX) || (INT_MIN != LONG_MIN)
    if (NTST_UNLIKELY((ret == 0) || (val > INT_MAX) || (val < INT_MIN)))
        return 0;
#else
    if (NTST_UNLIKELY(ret == 0))
        return 0;
#endif
    *result = (int)val;
    return ret;
    }

#define BUF_MAX_LEN_FOR_LONG sizeof(NTST_STRINGIZE_MACRO(LONG_MIN)) /* sizeof gives size with null terminator. */
size_t ntst_strn10tol(const char* NTST_RESTRICT str, size_t len, long* NTST_RESTRICT result)
    {
    /* +1 for extra character for possible overflow. */
    char buf[BUF_MAX_LEN_FOR_LONG + 1];
    size_t ret;
    /* Skip leading spaces */
    const char* begin = str;
    while (NTST_ISSPACE(*begin) && (len != 0))
        {
        ++begin;
        --len;
        }
    if (len > BUF_MAX_LEN_FOR_LONG)
        len = BUF_MAX_LEN_FOR_LONG;
    memcpy(buf, begin, len);
    buf[len] = '\0';
    ret = ntst_str10tol(buf, result);
    if (ret == 0)
        return 0;
    ret += (begin - str);
    return ret;
    }

size_t ntst_strn10tou(const char* NTST_RESTRICT str, size_t len, unsigned* NTST_RESTRICT result)
    {
    /* +1 for extra character for possible overflow.
       Buffer for long is long enough for unsigned(>=). */
    char buf[BUF_MAX_LEN_FOR_LONG + 1];
    size_t ret;
    /* Skip leading spaces */
    const char* begin = str;
    while (NTST_ISSPACE(*begin) && (len != 0))
        {
        ++begin;
        --len;
        }
    if (len > BUF_MAX_LEN_FOR_LONG)
        len = BUF_MAX_LEN_FOR_LONG;
    memcpy(buf, begin, len);
    buf[len] = '\0';
    ret = ntst_str10tou(buf, result);
    if (ret == 0)
        return 0;
    ret += (begin - str);
    return ret;
    }

#if (defined(__STDC__) && (__STDC_VERSION__ >= 199901L)) || (defined(__cplusplus) && (__cplusplus >= 201103L))
/* strtof - C99 || C++11 */
#define STROF strtof
#define STROF_TYPE float
#else
#define STROF strtod
#define STROF_TYPE double
#endif
    
size_t ntst_strtof(const char* NTST_RESTRICT str, float* NTST_RESTRICT result)
    {
    char *str_end;
    STROF_TYPE res;
    assert(str != NULL);
    assert(result != NULL);
    errno = 0;
    res = STROF(str, &str_end);
    if (NTST_UNLIKELY((str_end == str) || (errno != 0)))
        return 0;
    *result = (float)res;
    return (str_end - str);
    }

size_t ntst_strtod(const char* NTST_RESTRICT str, double* NTST_RESTRICT result)
    {
    char *str_end;
    double res;
    assert(str != NULL);
    assert(result != NULL);
    errno = 0;
    res = strtod(str, &str_end);
    if (NTST_UNLIKELY((str_end == str) || (errno != 0)))
        return 0;
    *result = res;
    return (str_end - str);
    }

void ntst_blobtohexstr(const void* NTST_RESTRICT blob, size_t blob_size, char* NTST_RESTRICT str)
    {
    size_t i;
    const unsigned char* blob_str = (const unsigned char*)blob;
    for (i = 0; i < blob_size; ++i)
        {
        ntst_byte_to2char(blob_str[i], &str[2 * i]);
        }
    }

static size_t ntst_escape_char_do(char c, void* out, bool do_escape)
    {
    size_t result;
    char* str = (char*)out;
    switch (c)
        {
        case '\n':
            str[0] = '\\';
            str[1] = 'n';
            result = 2;
            break;
        case '\t':
            str[0] = '\\';
            str[1] = 't';
            result = 2;
            break;
        case '\v':
            str[0] = '\\';
            str[1] = 'v';
            result = 2;
            break;
        case '\b':
            str[0] = '\\';
            str[1] = 'b';
            result = 2;
            break;
        case '\r':
            str[0] = '\\';
            str[1] = 'r';
            result = 2;
            break;
        case '\f':
            str[0] = '\\';
            str[1] = 'f';
            result = 2;
            break;
        case '\a':
            str[0] = '\\';
            str[1] = 'a';
            result = 2;
            break;
        case '\\':
            str[0] = '\\';
            str[1] = '\\';
            result = 2;
            break;
        default:
#if !defined(NTST_ASCII_ENCODING)
#error Not expected system encoding
#endif
            if (NTST_LIKELY(!do_escape))
                {
                str[0] = c;
                result = 1;
                }
            else
                {
                str[0] = '\\';
                ntst_byte_to2char(c, &str[1]);
                result = 3;
                }
            break;
        }
    assert(result > 0);
    assert(result <= NTST_ESCAPED_CHAR_MAX_SIZE);
    return result;
    }

/* space is first printable */
size_t ntst_escape_char(char c, void* out)
    {
    return ntst_escape_char_do(c, out, (unsigned char)c < ' ');
    }

size_t ntst_escape_char_ascii(char c, void* out)
    {
    return ntst_escape_char_do(c, out, (signed char)c < ' ');
    }

size_t ntst_blobtoescapedstr(const void* NTST_RESTRICT blob, size_t blob_size, char* NTST_RESTRICT str)
    {
    size_t i;
    size_t result = 0;
    const char* blob_str = (const char*)blob;
    for (i = 0; i < blob_size; ++i)
        {
        size_t cur_size = ntst_escape_char_ascii(blob_str[i], &str[result]);
        result += cur_size;
        }
    return result;
    }

char* ntst_blob_to_str(const char* NTST_RESTRICT blob_name, const void* NTST_RESTRICT blob, size_t* NTST_RESTRICT size)
    {
    const size_t blob_size = *size;
    const size_t blob_2size = blob_size * 2;
    char* blob_hex_str;
    char* full_str = NULL;

    blob_hex_str = (char*)NTST_MALLOC(blob_2size + NTST_ONE_NULL_TERM);
    if (NTST_LIKELY(blob_hex_str != NULL))
        {
        const size_t blob_esc_max_size = blob_size * NTST_ESCAPED_CHAR_MAX_SIZE;
        char* blob_esc_str = (char*)NTST_MALLOC(blob_esc_max_size + NTST_ONE_NULL_TERM);
        if (NTST_LIKELY(blob_esc_str != NULL))
            {
            size_t esc_len;
            int full_len;
            ntst_blobtohexstr(blob, blob_size, blob_hex_str);
            blob_hex_str[blob_2size] = '\0';
            esc_len = ntst_blobtoescapedstr(blob, blob_size, blob_esc_str);
            blob_esc_str[esc_len] = '\0';
/* name size hex escaped_str */
#define BLOB_FORMAT "%s s:%u b:%s %s"
            full_len = ntst_snprintf(NULL, 0, BLOB_FORMAT, blob_name, (unsigned)blob_size, blob_hex_str, blob_esc_str);
            if (full_len > 0)
                {
                size_t full_size = full_len + NTST_ONE_NULL_TERM;
                full_str = (char*)NTST_MALLOC(full_size);
                if (full_str != NULL)
                    {
                    ntst_snprintf(full_str, full_size, BLOB_FORMAT, blob_name, (unsigned)blob_size, blob_hex_str, blob_esc_str);
                    *size = full_size;
                    }
                }
            NTST_FREE(blob_esc_str);
            }
        NTST_FREE(blob_hex_str);
        }
    return full_str;
    }

char* ntst_strchr_after_n(const char* str, int c, unsigned num)
    {
    unsigned i;
    char* result = (char*)str;
    for (i = 0; i < num; ++i)
        {
        result = strchr(result, c);
        if (result == NULL)
            return NULL;
        ++result;
        }
    return result;
    }

char* ntst_strnchr(const char* str, int ch, size_t n)
    {
    const char* end = str + n;
    while (str != end)
        {
        if (NTST_UNLIKELY(*str == ch))
            return (char*)str;
        if (NTST_UNLIKELY(*str == '\0'))
            break;
        ++str;
        }
    return NULL;
    }

size_t ntst_strchr_cnt(const char* str, char c)
    {
    size_t cnt = 0;
    while (*str != '\0')
        {
        if (NTST_UNLIKELY(*str == c))
            ++cnt;
        ++str;
        }
    return cnt;
    }

size_t ntst_strnlen(const char* str, size_t n)
    {
    const char* end = str + n;
    const char* cur = str;
    while (cur != end)
        {
        if (NTST_UNLIKELY(*cur == '\0'))
            return (cur - str);
        ++cur;
        }
    return n;
    }

void* ntst_memchr(const void* NTST_RESTRICT data, size_t len, char c)
{
    const char* cur = data;
    const char* end = cur + len;
    while (cur != end)
    {
        if (NTST_UNLIKELY(*cur == c))
            return (void*)cur;
        ++cur;
    }
    return NULL;
}

void* ntst_memchr_skipn(const void* NTST_RESTRICT data, size_t len, char c, unsigned n)
{
    return ntst_memchr_it_skipn(data, (const char*)data + len, c, n);
}

void* ntst_memchr_it_skipn(const void* NTST_RESTRICT dataBegin, const void* NTST_RESTRICT dataEnd, char c, unsigned n)
{
    const char* cur = dataBegin;
    const char* end = dataEnd;
    while (cur != end)
    {
        if (NTST_UNLIKELY(*cur == c))
        {
            if (n == 0)
                return (void*)cur;
            else
                --n;
        }
        ++cur;
    }
    return NULL;
}

void* ntst_memmem(const void* NTST_RESTRICT data, size_t len, const void* NTST_RESTRICT subdata, size_t sublen)
    {
    if (NTST_LIKELY(len >= sublen))
        {
        const char* cur = (const char*)data;
        const char* end = cur + len - sublen + 1;
        while (cur != end)
            {
            if (NTST_UNLIKELY(memcmp(cur, subdata, sublen) == 0))
                return (void*)cur;
            ++cur;
            }
        }
    return NULL;
    }

unsigned ntst_mem_fixedp_to_uint(const void* NTST_RESTRICT data, size_t len, struct FixedPointInfo* NTST_RESTRICT out)
{
    return ntst_mem_it_fixedp_to_uint(data, (const char*)data + len, out);
}

unsigned ntst_mem_it_fixedp_to_uint(const void* NTST_RESTRICT dataBegin, const void* NTST_RESTRICT dataEnd, struct FixedPointInfo* NTST_RESTRICT out)
{
    unsigned ret = 0;
    const char* begin = (const char*)dataBegin;
    const char* cur = begin;
    const char* end = (const char*)dataEnd;
    const char* sepPtr = NULL;
    out->separator = '\0'; // Выставляем как признак что ещё не встречали.
    while (cur != end)
    {
        const char c = *cur;
        if (NTST_LIKELY(NTST_ISDIGIT(c)))
        { // TODO по хорошему тут надо бы переполнение проверять.
            // В C23 должно появиться <stdckdint.h> ckd_mul. В GCC __builtin_mul_overflow
            // Для старья можно сравнивать с UINT_MAX/10 до умножения. Если <= то можно умножать
            // Кстати если буду делать вариант для int, то его переполнение вроде UB, поэтому надо будет тестировать не вызывая.
            // Проще всего запоминать минус, парсить как unsigned и смотреть диапазон.
            ret *= 10;
            ret += c - '0';
        }
        else
        {
            if (out->separator == '\0')
            { // Встретили разделитель
                sepPtr = cur;
                if (NTST_UNLIKELY(c == '\0'))
                    break; // Разделитель внезапно конец строки. Выходим.
                out->separator = c;
            }
            else
            { // Второе не число, значит закончилась дробная часть.
                break;
            }
        }
        ++cur;
    }
    if (NTST_UNLIKELY(sepPtr == NULL))
        sepPtr = cur; // Возможно если дошли до конца(cur == end) не встретив разделителя.
    out->totalLen = cur - begin;
    out->partLen = cur - sepPtr;
    return ret;
}

#if defined(_MSC_VER)
#pragma warning(disable: 4244)
#endif
static double ntst_size_to_pretty(double divisor, long long bytes, unsigned suffixes_len, unsigned* suffix_index)
{
    double ret;
    unsigned suffixes_max;
    assert(suffixes_len > 0);
    *suffix_index = 0;
    ret = bytes;
    suffixes_max = suffixes_len - 1;
    while (ret >= 1024 && *suffix_index < suffixes_max)
    {
        ++(*suffix_index);
        ret /= divisor;
    }
    return ret;
}

double ntst_size_to_pretty1000(long long bytes, unsigned suffixes_len, unsigned* suffix_index)
{
    return ntst_size_to_pretty(1000, bytes, suffixes_len, suffix_index);
}

double ntst_size_to_pretty1024(long long bytes, unsigned suffixes_len, unsigned* suffix_index)
{
    return ntst_size_to_pretty(1024, bytes, suffixes_len, suffix_index);
}

static int ntst_pretty_print_size(double divisor, long long bytes, char* NTST_RESTRICT buff, unsigned buff_len, const char* NTST_RESTRICT format, const char** NTST_RESTRICT suffixes, unsigned suffixes_len)
    {
    double val;
    unsigned suf;
    assert(suffixes_len > 0);
    val = ntst_size_to_pretty(divisor, bytes, suffixes_len, &suf);
    return ntst_snprintf(buff, buff_len, format, val, suffixes[suf]);
    }

int ntst_pretty_print_size1000(long long bytes, char* NTST_RESTRICT buff, unsigned buff_len, const char* NTST_RESTRICT format, const char** NTST_RESTRICT suffixes, unsigned suffixes_len)
    {
    return ntst_pretty_print_size(1000, bytes, buff, buff_len, format, suffixes, suffixes_len);
    }

int ntst_pretty_print_size1024(long long bytes, char* NTST_RESTRICT buff, unsigned buff_len, const char* NTST_RESTRICT format, const char** NTST_RESTRICT suffixes, unsigned suffixes_len)
    {
    return ntst_pretty_print_size(1024, bytes, buff, buff_len, format, suffixes, suffixes_len);
    }
