#include <assert.h>

#include "ntst_base.h"
#include "ntst_error.h"
#include "ntst_thread.h"
#include "ntst_string.h" /* ntst_snprintf */

#define ERRNO_OFFSET 16
#ifdef NTST_THREAD_LOCAL
NTST_THREAD_LOCAL char ntst_errbuf[200];
#endif

#define ERR_CASE(x) case x: return NTST_ERROR_##x;

ntst_error_t ntst_error_from_errno(int e)
    {
    int ret;
    assert(e >= 0);
    switch (e)
        {
        case 0: return NTST_ERROR_OK;
        ERR_CASE(EINTR)
        ERR_CASE(EINVAL)
        ERR_CASE(EOVERFLOW)
        ERR_CASE(EAGAIN)
        ERR_CASE(ENOMEM)
        ERR_CASE(EBADF)
        ERR_CASE(ENOENT)
        ERR_CASE(EINPROGRESS)
#if NTST_ERROR_EAGAIN != NTST_ERROR_EWOULDBLOCK
        ERR_CASE(EWOULDBLOCK)
#endif
        ERR_CASE(ENOTCONN)
        }
    ret = e << ERRNO_OFFSET;
    assert((ret >> ERRNO_OFFSET) == e);
    return ret;
    }

#undef ERR_CASE

#if defined(_WIN32)

ntst_error_t ntst_error_from_system(DWORD e)
    {
    switch (e)
        {
        case 0:
            return NTST_ERROR_OK;
        }
    return -((ntst_error_t)e);
    }

#else

#include <string.h> /* strerror */

ntst_error_t ntst_error_from_system(int e)
    {
    return ntst_error_from_errno(e);
    }

#endif

static const char* ntst_strerror(int e)
    {
    /* TODO POSIX: The strerror() function need not be reentrant. A function that is not required to be reentrant is not required to be thread-safe.
    also: no return value is reserved to indicate an error, an application wishing to check for error situations should set errno to 0, then call strerror(), then check errno*/
    const char* result = strerror(e);
    /* if someone uses NULL as return value on error */
    if (NTST_LIKELY(result != NULL))
        return result;
    return "strerror NULL";
    }

#if defined(_WIN32)

const char* ntst_error_win_to_string(DWORD e)
    {
    char* tmp_buffer;
    DWORD len = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL, e,
                   MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                   (LPSTR)&tmp_buffer, 0, NULL);
    if (NTST_UNLIKELY(len == 0))
        {
        ntst_snprintf(ntst_errbuf, NTST_ARRAY_LEN(ntst_errbuf), "FormatMessage failed:%u", (unsigned)e);
        return ntst_errbuf;
        }
    while (len > 0)
        {
        const int c = tmp_buffer[--len];
        if (c == '\r' || c == '\n')
            tmp_buffer[len] = '\0';
        else
            break;
        }
    ntst_snprintf(ntst_errbuf, NTST_ARRAY_LEN(ntst_errbuf), "code(%d):%s", (int)e, (const char*)tmp_buffer);
    LocalFree(tmp_buffer);
    return ntst_errbuf;
    }
#endif

#define ERR_CASE(x) case NTST_ERROR_##x: return "NTST_"#x;

const char* ntst_error_to_string(int e)
    {
    if (e >= 0)
        {
        switch (e)
            {
            ERR_CASE(OK)
            ERR_CASE(EINTR)
            ERR_CASE(EINVAL)
            ERR_CASE(EOVERFLOW)
            ERR_CASE(ENOMEM)
            ERR_CASE(EAGAIN)
            ERR_CASE(EBADF)
            ERR_CASE(ENOENT)
            ERR_CASE(EINPROGRESS)
#if NTST_ERROR_EAGAIN != NTST_ERROR_EWOULDBLOCK
            ERR_CASE(EWOULDBLOCK)
#endif
            ERR_CASE(ENOTCONN)
            }
        if (e >= (1 << ERRNO_OFFSET))
            {
            const int e_errno = (unsigned)e >> ERRNO_OFFSET;
#ifdef NTST_THREAD_LOCAL
            ntst_snprintf(ntst_errbuf, NTST_ARRAY_LEN(ntst_errbuf), "errno(%d):%s", e_errno, ntst_strerror(e_errno));
            return ntst_errbuf;
#else
            return ntst_strerror(e_errno);
#endif
            }
#ifdef NTST_THREAD_LOCAL
        ntst_snprintf(ntst_errbuf, NTST_ARRAY_LEN(ntst_errbuf), "NTST_ERROR_NOT_EXPECTED(%d)", e);
        return ntst_errbuf;
#else
        return "NTST_ERROR_NOT_EXPECTED";
#endif
        }
    else /* e < 0 */
        {
#if defined(_WIN32)
        return ntst_error_win_to_string(-e);
#else
#   ifdef NTST_THREAD_LOCAL
        ntst_snprintf(ntst_errbuf, NTST_ARRAY_LEN(ntst_errbuf), "NTST_ERROR_NOT_EXPECTED(%d)", e);
        return ntst_errbuf;
#   else
        return "NTST_ERROR_NOT_EXPECTED_NEGATIVE";
#   endif
#endif
        }
    }
