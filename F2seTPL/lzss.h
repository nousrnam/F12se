#ifndef lzssH
#define lzssH

#include "commontypes.h"

int decompress_lzss(uint8 *dst, uint8 *src, uint32 srclen);
#endif
