#ifndef GNTST_THREAD_H
#define GNTST_THREAD_H

#include "ntst_error.h"

#if defined(_MSC_VER)
/* Since Visual Studio 2003 */
#define NTST_THREAD_LOCAL __declspec(thread)
#else
#define NTST_THREAD_LOCAL __thread
/* In C11, the keyword _Thread_local is used to define thread-local variables */
/* C++11 introduces the thread_local */
/* G++ supports thread_local since gcc 4.8. Unfortunately, this support requires a run-time penalty for references to non-function-local thread_local variables even if they don't need dynamic initialization, so users may want to continue to use __thread for TLS variables with static initialization semantics. */
/*#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 8)))
#define NTST_THREAD_LOCAL thread_local*/
#endif

#if defined(_WIN32)
#include <windows.h>
typedef DWORD ntst_thread_id_t;
#define ntst_get_thread_id GetCurrentThreadId
#elif defined(__linux__)
#include <sys/types.h>
#include <sys/syscall.h>
typedef pid_t ntst_thread_id_t;
/* gettid often has some problem.*/
#if defined(SYS_gettid)
#define ntst_get_thread_id() (pid_t)syscall(SYS_gettid)
#else
#define ntst_get_thread_id gettid
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Beware: can return before the requested time has elapsed. */
ntst_error_t ntst_sleep_millisec(unsigned milliseconds);

/*
Win32 threads do not have names. But with Visual Studio it is possible to tell name to debugger.
Posix names are restricted to 16 characters, including '\0'. So ..._auto_truncate version. On linux "string is silently truncated"
*/
ntst_error_t ntst_set_current_thread_name(const char* name);
#if defined(_WIN32) || defined(__linux__)
#define ntst_set_current_thread_name_auto_truncate ntst_set_current_thread_name
#else
ntst_error_t ntst_set_current_thread_name_auto_truncate(const char* name);
#endif

/*
Warning!
Linux:
Avoid calling sched_yield() unnecessarily or inappropriately (e.g., when
resources needed by other schedulable threads are still held by the
caller), since doing so will result in unnecessary context switches,
which will degrade system performance.
!!!degrade system performance!!! - Seems like I did it.
*/
ntst_error_t ntst_thread_yield();

#ifdef __cplusplus
}
#endif

#endif
