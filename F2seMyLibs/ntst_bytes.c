#include "ntst_base.h"
#include "ntst_bytes.h"

#if defined _MSC_VER
#include <assert.h>
#include <stdlib.h> /* _byteswap... */
#endif

uint16_t ntst_bytes_swap16(uint16_t val)
    {
#ifdef __GNUC__
    return __builtin_bswap16(val);
#elif defined _MSC_VER
    ntst_static_assert(sizeof(unsigned short) == 2, "Replace _byteswap_ushort with 16-bit equivalent.");
    return _byteswap_ushort(val);
#else
    return ((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8);
#endif
    }

uint32_t ntst_bytes_swap32(uint32_t val)
    {
#ifdef __GNUC__
    return __builtin_bswap32(val);
#elif defined _MSC_VER
    ntst_static_assert(sizeof(unsigned long) == 4, "Replace _byteswap_ulong with 32-bit equivalent.");
    return _byteswap_ulong(val);
#else
    return (val >> 24) | (val << 24) | ((val >> 8) & 0x0000FF00) | ((val << 8) & 0x00FF0000);
#endif
    }

uint64_t ntst_bytes_swap64(uint64_t val)
    {
#ifdef __GNUC__
    return __builtin_bswap64(val);
#elif defined _MSC_VER
    return _byteswap_uint64(val);
#else
    val = (val & 0x00000000FFFFFFFF) << 32 | (val & 0xFFFFFFFF00000000) >> 32;
    val = (val & 0x0000FFFF0000FFFF) << 16 | (val & 0xFFFF0000FFFF0000) >> 16;
    val = (val & 0x00FF00FF00FF00FF) << 8  | (val & 0xFF00FF00FF00FF00) >> 8;
    return val;
#endif
    }

size_t ntst_buf_cmp(const void* buf1, const void* buf2, size_t buf_size)
    {
    size_t i;
    const char* buf1_ = (const char*)buf1;
    const char* buf2_ = (const char*)buf2;
    for (i = 0; i < buf_size; ++i)
        {
        if (buf1_[i] != buf2_[i])
            return i;
        }
    return buf_size;
    }

void ntst_reverse_bytes_it(void* begin, void* end)
    {
    char* b = (char*)begin;
    char* e = (char*)end;
    while (b < e)
        {
        const char temp = *b;
        --e;
        *b = *e;
        *e = temp;
        ++b;
        }
    }

void ntst_reverse_bytes_len(void* buf, size_t size)
    {
    ntst_reverse_bytes_it(buf, (char*)buf + size);
    }

void ntst_memcopy_reversed(void* NTST_RESTRICT dest, const void* NTST_RESTRICT src, size_t count)
    {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    size_t is = 0, id = count - 1;
    for (; is < count; ++is, --id)
        d[id] = s[is];
    }
