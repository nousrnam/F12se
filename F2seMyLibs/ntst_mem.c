#include "ntst_mem.h"

#if !defined(NTST_MALLOC) && !defined(NTST_FREE) && !defined(NTST_REALLOC) && defined(ENTST_ENABLE_DEBUG)
static int allocated_num = 0;
/* TODO make allocated_num operations atomic */
/* TODO check in ntst_free_dbg memory was allocated before */
/* TODO modify debug macro with __FILE__ __LINE__ */
void* ntst_malloc_dbg(size_t size)
    {
    void* ret = malloc(size);
    if (ret)
        {
        ++allocated_num;
        }
    return ret;
    }

void ntst_free_dbg(void* ptr)
    {
    free(ptr);
    --allocated_num;
    }

void* ntst_realloc_dbg(void* ptr, size_t new_size)
    {
    return realloc(ptr, size);
    }

#endif
