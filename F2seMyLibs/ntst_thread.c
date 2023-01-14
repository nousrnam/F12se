#include "ntst_thread.h"
#include "ntst_base.h"

#if defined(_WIN32)

#if defined (_MSC_VER)
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
 } THREADNAME_INFO;
#pragma pack(pop)

ntst_error_t ntst_set_current_thread_name(const char* name)
    {
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = -1;
    info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
    __try
        {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
        }
    __except (EXCEPTION_EXECUTE_HANDLER)
        {}
#pragma warning(pop)
    return NTST_ERROR_OK;
    }
#endif

ntst_error_t ntst_sleep_millisec(unsigned milliseconds)
    {
    if (NTST_LIKELY(SleepEx(milliseconds, TRUE) == 0))
        return NTST_ERROR_OK;
    return NTST_ERROR_EINTR;
    }

ntst_error_t ntst_thread_yield()
    {
    SwitchToThread();
    return NTST_ERROR_OK;
    }

#else

#if defined(__linux__)
#include <sys/prctl.h>  /* prctl(), PR_SET_NAME */

ntst_error_t ntst_set_current_thread_name(const char* name)
    {
    if (NTST_LIKELY(prctl(PR_SET_NAME, (unsigned long) name) == 0))
        return NTST_ERROR_OK;
    return ntst_error_from_errno(errno);
    }

#else
#include <pthread.h>
/*
https://stackoverflow.com/questions/2369738/can-i-set-the-name-of-a-thread-in-pthreads-linux
// Linux
int pthread_setname_np(pthread_t thread, const char *name);
// NetBSD: name + arg work like printf(name, arg)
int pthread_setname_np(pthread_t thread, const char *name, void *arg);
// FreeBSD & OpenBSD: function name is slightly different, and has no return value
void pthread_set_name_np(pthread_t tid, const char *name);
// Mac OS X: must be set from within the thread (can't specify thread ID)
int pthread_setname_np(const char*);

TODO when required
ntst_error_t ntst_set_current_thread_name(const char* name)
    {
    }
*/

#ifdef PTHREAD_MAX_NAMELEN_NP
#define MAX_NAMELEN PTHREAD_MAX_NAMELEN_NP
#else
#define MAX_NAMELEN 16
#endif

ntst_error_t ntst_set_current_thread_name_auto_truncate(const char* name)
    {
    char buf[MAX_NAMELEN];
    unsigned i;
    for (i = 0; i < MAX_NAMELEN - 1; ++i)
        {
        if (name[i] == '\0')
            break;
        buf[i] = name[i];
        }
    buf[i] = '\0';
    return ntst_set_current_thread_name(buf);
    }

#endif

#include <features.h>
#include <time.h> /* nanosleep */
#include <sched.h>

#if _POSIX_C_SOURCE >= 199309L

ntst_error_t ntst_sleep_millisec(unsigned milliseconds)
    {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (long)(milliseconds % 1000) * 1000000;
    /* NULL, the remaining time is not returned */
    if (NTST_LIKELY(nanosleep(&ts, NULL) == 0))
        return NTST_ERROR_OK;
    return ntst_error_from_errno(errno);
    }
#else
#include <unistd.h> /* usleep */
/* usleep is obsolete */
ntst_error_t ntst_sleep_millisec(unsigned milliseconds)
    {
    if (NTST_LIKELY(usleep((useconds_t)milliseconds * 1000) == 0))
        return NTST_ERROR_OK;
    return ntst_error_from_errno(errno);
    }
#endif /* _POSIX_C_SOURCE >= 199309L */

ntst_error_t ntst_thread_yield()
    {
    if (NTST_LIKELY(sched_yield() == 0))
        return NTST_ERROR_OK;
    return ntst_error_from_errno(errno);
    }

#endif /* defined(_WIN32) */
