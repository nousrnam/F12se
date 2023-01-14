#ifndef GNTST_BYTES_H
#define GNTST_BYTES_H

#include <stdint.h> /* uint.._t */
#include <limits.h> /* CHAR_BIT */
#include "ntst_base.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t ntst_bytes_swap16(uint16_t val);
uint32_t ntst_bytes_swap32(uint32_t val);
uint64_t ntst_bytes_swap64(uint64_t val);

/* TODO compile time detection */
#define NTST_INT16_TO_LE(x) (x)
#define NTST_INT16_TO_BE(x) ((int16_t)ntst_bytes_swap16(x))
#define NTST_UINT16_TO_LE(x) (x)
#define NTST_UINT16_TO_BE(x) (ntst_bytes_swap16(x))
#define NTST_INT32_TO_LE(x) (x)
#define NTST_INT32_TO_BE(x) ((int32_t)ntst_bytes_swap32(x))
#define NTST_INT64_TO_LE(x) (x)
#define NTST_INT64_TO_BE(x) ((int64_t)ntst_bytes_swap64(x))
#define NTST_UINT32_TO_LE(x) (x)
#define NTST_UINT32_TO_BE(x) (ntst_bytes_swap32(x))
#define NTST_UINT64_TO_LE(x) (x)
#define NTST_UINT64_TO_BE(x) (ntst_bytes_swap64(x))

size_t ntst_buf_cmp(const void* buf1, const void* buf2, size_t buf_size);
void ntst_reverse_bytes_it(void* begin, void* end);
void ntst_reverse_bytes_len(void* buf, size_t size);
void ntst_memcopy_reversed(void* NTST_RESTRICT dest, const void* NTST_RESTRICT src, size_t count);

#if defined(_MSC_VER)
/* inline is C99 and not supported by microsoft c compiler */
#define inline __inline
#endif

static inline unsigned ntst_read_uint_2le(const void* src){const unsigned char* s=(const unsigned char*)src;return s[0]|(s[1]<<CHAR_BIT);}
static inline unsigned ntst_read_uint_3le(const void* src){const unsigned char* s=(const unsigned char*)src;return s[0]|(s[1]<<CHAR_BIT)|(s[2]<<(2*CHAR_BIT));}
static inline unsigned ntst_read_uint_4le(const void* src){const unsigned char* s=(const unsigned char*)src;return s[0]|(s[1]<<CHAR_BIT)|(s[2]<<(2*CHAR_BIT))|(s[3]<<(3*CHAR_BIT));}
static inline unsigned ntst_read_uint_2be(const void* src){const unsigned char* s=(const unsigned char*)src;return (s[0]<<CHAR_BIT)|s[1];}
static inline unsigned ntst_read_uint_3be(const void* src){const unsigned char* s=(const unsigned char*)src;return (s[0]<<(2*CHAR_BIT))|(s[1]<<CHAR_BIT)|s[2];}
static inline unsigned ntst_read_uint_4be(const void* src){const unsigned char* s=(const unsigned char*)src;return (s[0]<<(3*CHAR_BIT))|(s[1]<<(2*CHAR_BIT))|(s[2]<<CHAR_BIT)|s[3];}
static inline void ntst_write_uint_2le(void* dst, unsigned val){unsigned char* d=(unsigned char*)dst;d[0]=val;d[1]=val>>CHAR_BIT;}
static inline void ntst_write_uint_3le(void* dst, unsigned val){unsigned char* d=(unsigned char*)dst;d[0]=val;d[1]=val>>CHAR_BIT;d[2]=val>>(2*CHAR_BIT);}
static inline void ntst_write_uint_4le(void* dst, unsigned val){unsigned char* d=(unsigned char*)dst;d[0]=val;d[1]=val>>CHAR_BIT;d[2]=val>>(2*CHAR_BIT);d[3]=val>>(3*CHAR_BIT);}
static inline void ntst_write_uint_2be(void* dst, unsigned val){unsigned char* d=(unsigned char*)dst;d[0]=val>>CHAR_BIT;d[1]=val;}
static inline void ntst_write_uint_3be(void* dst, unsigned val){unsigned char* d=(unsigned char*)dst;d[0]=val>>(2*CHAR_BIT);d[1]=val>>CHAR_BIT;d[2]=val;}
static inline void ntst_write_uint_4be(void* dst, unsigned val){unsigned char* d=(unsigned char*)dst;d[0]=val>>(3*CHAR_BIT);d[1]=val>>(2*CHAR_BIT);d[2]=val>>CHAR_BIT;d[3]=val;}

#if defined(_MSC_VER)
#undef inline
#endif

#ifdef __cplusplus
}
namespace ntst
{
template<typename SignedInt>
unsigned EncodeSLEB128(SignedInt val, void* buf)
    {
    unsigned char* start = (unsigned char*)buf;
    unsigned char* cur_ptr = (unsigned char*)buf;
    bool more;
    do
        {
        uint_fast8_t byte = val & 0x7F;
        /* TODO need static check signed right shift is an arithmetic. */
        val >>= 7;
        const bool signBitIsClear = ((byte & 0x40) == 0);
        more = !(((val == 0) && signBitIsClear) ||
              ((val == -1) && !signBitIsClear));
        if (buf != NULL)
            {
            if (more)
                byte |= 0x80;
            *cur_ptr = byte;
            }
        ++cur_ptr; // Need this to return size even for NULL
        }
    while (more);
    return (cur_ptr - start);
    }

template<typename UnsignedInt>
unsigned EncodeULEB128(UnsignedInt val, void* buf)
    {
    unsigned char* start = (unsigned char*)buf;
    unsigned char* cur_ptr = (unsigned char*)buf;
    do
        {
        uint_fast8_t byte = val & 0x7F;
        val >>= 7;
        if (buf != NULL)
            {
            if (val != 0)
                byte |= 0x80;
            *cur_ptr = byte;
            }
        ++cur_ptr; // Need this to return size even for NULL
        }
    while (val != 0);
    return (cur_ptr - start);
    }

template<typename SignedInt>
unsigned DecodeSLEB128(const void* buf, size_t buf_size, SignedInt& val)
    {
    val = 0;
    unsigned char* start = (unsigned char*)buf;
    unsigned char* cur_ptr = (unsigned char*)buf;
    unsigned shift = 0;
    unsigned size_bits = sizeof(val) * CHAR_BIT;
    uint_fast8_t byte;
    for (;;)
        {
        if (NTST_UNLIKELY(buf_size == 0))
            return 0;
        byte = *cur_ptr;
        val |= ((SignedInt)(byte & 0x7F) << shift);
        ++cur_ptr;
        if (byte <= 0x7F)
            break;
        shift += 7;
        --buf_size;
        }
    if ((shift < size_bits) && (byte & 0x40))
        val |= -((SignedInt)1 << shift); /* sign extend */
    return (cur_ptr - start);
    }

template<typename UnsignedInt>
unsigned DecodeULEB128(const void* buf, size_t buf_size, UnsignedInt& val)
    {
    val = 0;
    unsigned char* start = (unsigned char*)buf;
    unsigned char* cur_ptr = (unsigned char*)buf;
    unsigned shift = 0;
    for (;;)
        {
        if (NTST_UNLIKELY(buf_size == 0))
            return 0;
        unsigned char byte = *cur_ptr;
        val |= ((UnsignedInt)(byte & 0x7F) << shift);
        ++cur_ptr;
        if (byte <= 0x7F)
            break;
        shift += 7;
        --buf_size;
        }
    return (cur_ptr - start);
    }
}
#endif

#endif
