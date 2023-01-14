#include <assert.h>
#include <math.h> /* modf */
#include "ntst_time.h"
#include "ntst_string.h"

#if defined(_WIN32)
/* https://support.microsoft.com/en-us/kb/167296 */
#include <winbase.h>
#include <winnt.h>
#include <oleauto.h>

static void WinTimeToTm(const SYSTEMTIME* NTST_RESTRICT st, struct tm* NTST_RESTRICT tm)
    {
    assert(st != NULL);
    assert(tm != NULL);
    tm->tm_year = st->wYear - 1900;
    tm->tm_mon = st->wMonth - 1;
    tm->tm_mday = st->wDay;
    tm->tm_hour = st->wHour;
    tm->tm_min = st->wMinute;
    tm->tm_sec = st->wSecond;
    tm->tm_wday = st->wDayOfWeek;
    tm->tm_isdst = -1; /* A negative value of time->tm_isdst causes mktime to attempt to determine if Daylight Saving Time was in effect in the specified time. */
    }

static void TmToWinTime(const struct tm* NTST_RESTRICT tm, SYSTEMTIME* NTST_RESTRICT st)
    {
    assert(st != NULL);
    assert(tm != NULL);
    st->wYear = tm->tm_year + 1900;
    st->wMonth = tm->tm_mon + 1;
    st->wDay = tm->tm_mday;
    st->wDayOfWeek = tm->tm_wday;
    st->wHour = tm->tm_hour;
    st->wMinute = tm->tm_min;
    st->wSecond = tm->tm_sec;
    st->wMilliseconds = 0;
    }

void ntst_win_timet_to_filetime(time_t t, LPFILETIME pft)
    {
    /* Note that LONGLONG is a 64-bit value */
    LONGLONG ll;
    ll = Int32x32To64(t, 10000000) + 116444736000000000LL;
    pft->dwLowDateTime = (DWORD)ll;
    pft->dwHighDateTime = ll >> 32;
    }

void ntst_win_filetime_to_timet(LPFILETIME pft, time_t* t)
    {
    /* Note that LONGLONG is a 64-bit value */
    LONGLONG ll = (LONGLONG)pft->dwLowDateTime | (((LONGLONG)pft->dwHighDateTime) << 32);
    /* + 10000000LL/2 to round, not truncate */
    *t = (ll - 116444736000000000LL + 10000000LL/2) / 10000000LL;
    }

ntst_error_t ntst_win_systemtime_to_ntsttime(SYSTEMTIME* NTST_RESTRICT st, ntst_time_t* NTST_RESTRICT t)
    {
    struct tm tm;
    assert(st != NULL);
    assert(t != NULL);
    WinTimeToTm(st, &tm);
    t->tv_sec = mktime(&tm);
    if (NTST_LIKELY(t->tv_sec != -1))
        {
        t->tv_nsec = st->wMilliseconds * 1000000L;
        return NTST_ERROR_OK;
        }
    return NTST_ERROR_EINVAL;
    }

ntst_error_t ntst_win_oletime_to_ntsttime(double time, ntst_time_t* NTST_RESTRICT t)
    {
    typedef INT(WINAPI* VariantTimeToSystemTimeT)(DOUBLE, LPSYSTEMTIME);
    SYSTEMTIME st;
    VariantTimeToSystemTimeT conv;
    HMODULE ole = GetModuleHandleW(L"OleAut32.dll");
    if (ole == NULL)
        return ntst_error_from_system(GetLastError());
    conv = (VariantTimeToSystemTimeT)(GetProcAddress(ole, "VariantTimeToSystemTime"));
    if (conv == NULL)
        return ntst_error_from_system(GetLastError());
    if (conv(time, &st) != FALSE)
        return ntst_win_systemtime_to_ntsttime(&st, t);
    return NTST_ERROR_EINVAL;
    }

#define EPOCH_DIFF 0x019DB1DED53E8000LL /* 116444736000000000 nsecs */
#define RATE_DIFF 10000000 /* 100 nsecs */

/*typedef int64_t filetime_t; */

/* Convert a UNIX time_t into a Windows filetime_t */
/*filetime_t unixTimeToFileTime(time_t utime) {
        int64_t tconv = ((int64_t)utime * RATE_DIFF) + EPOCH_DIFF;
        return tconv;
}*/

/* Convert a Windows filetime_t into a UNIX time_t */
/*time_t fileTimeToUnixTime(filetime_t ftime) {
        int64_t tconv = (ftime - EPOCH_DIFF) / RATE_DIFF;
        return (time_t)tconv;
}
*/
#endif

ntst_error_t ntst_localtime(const time_t* NTST_RESTRICT time, struct tm* NTST_RESTRICT tm_struct)
    {
#if defined(_WIN32)
#if defined(_MSC_VER)
    errno_t err;
    /* https://msdn.microsoft.com/ru-ru/library/vstudio/a442x3ye%28v=vs.100%29.aspx */
    assert(time != NULL);
    assert(tm_struct != NULL);
    err = localtime_s(tm_struct, time);
    if (NTST_LIKELY(err == 0))
        return NTST_ERROR_OK;
    return ntst_error_from_errno(err);
#else /* MinGW most likely */
    const struct tm* tmp;
    assert(time != NULL);
    assert(tm_struct != NULL);
    tmp = localtime(time);
    if (NTST_LIKELY(tmp != NULL))
        {
        *tm_struct = *tmp;
        return NTST_ERROR_OK;
        }
    /* POSIX requires that this function sets errno to EOVERFLOW if it fails because the argument is too large.
     POSIX returns only EOVERFLOW. Others not guaranteed to set errno. */
    return NTST_ERROR_EOVERFLOW;
#endif /* defined(_MSC_VER) */
#else /* not _WIN32 */
    /* http://pubs.opengroup.org/onlinepubs/9699919799/functions/localtime.html */
    assert(time != NULL);
    assert(tm_struct != NULL);
    if (NTST_LIKELY(localtime_r(time, tm_struct) != NULL))
        return NTST_ERROR_OK;
    return ntst_error_from_system(errno);
#endif
    }

ntst_error_t ntst_gmtime(const time_t* NTST_RESTRICT time, struct tm* NTST_RESTRICT tm_struct)
    {
#if defined(_WIN32)
#if defined(_MSC_VER)
    /* https://msdn.microsoft.com/ru-ru/library/vstudio/3stkd9be%28v=vs.100%29.aspx */
    errno_t err;
    assert(time != NULL);
    assert(tm_struct != NULL);
    err = gmtime_s(tm_struct, time);
    if (NTST_LIKELY(err == 0))
        return NTST_ERROR_OK;
    return ntst_error_from_errno(err);
#else /* MinGW most likely */
    const struct tm* tmp;
    assert(time != NULL);
    assert(tm_struct != NULL);
    tmp = gmtime(time);
    if (NTST_LIKELY(tmp != NULL))
        {
        *tm_struct = *tmp;
        return NTST_ERROR_OK;
        }
    /* POSIX requires that this function sets errno to EOVERFLOW if it fails because the argument is too large.
     POSIX returns only EOVERFLOW. Others not guaranteed to set errno. */
    return NTST_ERROR_EOVERFLOW;
#endif /* defined(_MSC_VER) */
#else /* not _WIN32 */
    assert(time != NULL);
    assert(tm_struct != NULL);
    /* http://pubs.opengroup.org/onlinepubs/9699919799/functions/gmtime.html */
    if (NTST_LIKELY(gmtime_r(time, tm_struct) != NULL))
        return NTST_ERROR_OK;
    return ntst_error_from_system(errno);
#endif
    }

#if !defined(_WIN32)
#include <unistd.h> /* test timegm */
#include <stdlib.h> /* getenv */
#include <limits.h> /* the <features.h> header file does not exist on all platforms,
    so it cannot be included without further ado. However, since it is included
    by other GNU glibc header files, a better way to obtain the above-mentioned
    macros is to include the <limits.h> header file */
#endif

time_t ntst_timegm(struct tm* tm_struct)
    {
    assert(tm_struct != NULL);
#if defined(_WIN32)
#   if defined(__MINGW32__)
#warning If you are using ntst_timegm or ntst_time_from_calendar_gmt, you should really test them!!!
    /* MinGW doesn't have _mkgmtime */
    /* TODO check errors */
    {
    SYSTEMTIME st;
    FILETIME file_time;
    time_t ret;
    TmToWinTime(tm_struct, &st);
    SystemTimeToFileTime(&st, &file_time);
    ntst_win_filetime_to_timet(&file_time, &ret);
    return ret;
    }
    /*return mktime(&tm_local);*/
#   else
    /* https://msdn.microsoft.com/ru-ru/library/vstudio/2093ets1%28v=vs.100%29.aspx */
    return _mkgmtime(tm_struct);
#   endif
#elif (defined(_BSD_SOURCE) || defined(_SVID_SOURCE) || (defined(__GLIBC__) && ((__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 19))))
    /* Since glibc 2.19 by default */
    return timegm(tm_struct);
#else
#error Bad platform!
    /* Copy pasted from internet:
    For a portable version of timegm(), set the TZ environment variable to UTC, call mktime(3) and restore the value of TZ. Something like */
    time_t ret;
    char *tz;
    tz = getenv("TZ");
    setenv("TZ", "", 1);
    tzset();
    ret = mktime(tm_struct);
    if (tz)
        setenv("TZ", tz, 1);
    else
        unsetenv("TZ");
    tzset();
    return ret;
#endif
    }

#if defined(_WIN32)

#ifndef NTST_NO_STATIC_ASSERT
ntst_static_assert(sizeof(DWORD) >= sizeof(unsigned), "unexpected");
#endif

ntst_error_t ntst_timer_now(ntst_timer_t* timer)
    {
    assert(timer != NULL);
    /* timeGetTime or GetTickCount */
    *timer = GetTickCount();
    return NTST_ERROR_OK;
    }

void ntst_timer_set_long_ago(ntst_timer_t* timer)
    {
    assert(timer != NULL);
    ntst_timer_now(timer);
    *timer = (*timer - 0x7FFFFFFF);
    }

int ntst_timer_diff_ms(const ntst_timer_t* NTST_RESTRICT timer_end, const ntst_timer_t* NTST_RESTRICT timer_begin)
    {
    assert(timer_end != NULL);
    assert(timer_begin != NULL);
    return (*timer_end - *timer_begin);
    }

ntst_error_t ntst_timer_elapsed_ms(const ntst_timer_t* NTST_RESTRICT timer, unsigned* NTST_RESTRICT result)
    {
    assert(timer != NULL);
    assert(result != NULL);
    *result = (GetTickCount() - *timer);
    return NTST_ERROR_OK;
    }

ntst_error_t ntst_timer_elapsed_ms_set_now(ntst_timer_t* NTST_RESTRICT timer, unsigned* NTST_RESTRICT result)
    {
    DWORD now = GetTickCount();
    assert(timer != NULL);
    assert(result != NULL);
    *result = (now - *timer);
    *timer = now;
    return NTST_ERROR_OK;
    }

void ntst_timer_add_msec_ip(ntst_timer_t* timer, int val)
    {
    *timer += val;
    }

ntst_error_t ntst_time_now(ntst_time_t* nt)
    {
    SYSTEMTIME st;
    assert(nt != NULL);
    GetLocalTime(&st);
    return ntst_win_systemtime_to_ntsttime(&st, nt);
    }
    
#else

#include <sys/time.h> /* gettimeofday() */
#include <limits.h> /* UINT_MAX */
#include <stdlib.h> /* div */

#define TIMESPEC_TO_MS(ts) (ts.tv_sec * 1000l + ts.tv_nsec / 1000000l)
#define TIMESPEC_PTR_TO_MS(ts) (ts->tv_sec * 1000l + ts->tv_nsec / 1000000l)

ntst_error_t ntst_timer_now(ntst_timer_t* timer)
    {
    /* A return value of 0 shall indicate that the call succeeded.
     * A return value of -1 shall indicate that an error occurred,
     * and errno shall be set to indicate the error. */
    if (NTST_LIKELY(clock_gettime(CLOCK_MONOTONIC, timer) == 0))
        {
        return NTST_ERROR_OK;
        }
    /* The gettimeofday() function shall return 0 and no value shall be reserved to indicate an error. */
    struct timeval t;
    gettimeofday(&t, NULL);
    timer->tv_sec = t.tv_sec;
    timer->tv_nsec = t.tv_usec * 1000;
    return NTST_ERROR_OK;
    }

void ntst_timer_set_long_ago(ntst_timer_t* timer)
    {
    ntst_timer_now(timer);
    timer->tv_sec -= (UINT_MAX / 1000);
    }

int ntst_timer_diff_ms(const ntst_timer_t* NTST_RESTRICT timer_end, const ntst_timer_t* NTST_RESTRICT timer_begin)
    {
    ntst_timer_t tmp;
    assert(timer_end != NULL);
    assert(timer_begin != NULL);
    tmp.tv_sec = timer_end->tv_sec - timer_begin->tv_sec;
    tmp.tv_nsec = timer_end->tv_nsec - timer_begin->tv_nsec;
    return TIMESPEC_TO_MS(tmp);
    }

ntst_error_t ntst_timer_elapsed_ms(const ntst_timer_t* NTST_RESTRICT timer, unsigned* NTST_RESTRICT result)
    {
    ntst_timer_t now;
    /*ntst_error_t err =*/ ntst_timer_now(&now);
    /*if (NTST_LIKELY(err == NTST_ERROR_OK))
        {*/
        now.tv_sec -= timer->tv_sec;
        now.tv_nsec -= timer->tv_nsec;
        *result = TIMESPEC_TO_MS(now);
        return NTST_ERROR_OK;
        /*}
    return err;*/
    }

ntst_error_t ntst_timer_elapsed_ms_set_now(ntst_timer_t* NTST_RESTRICT timer, unsigned* NTST_RESTRICT result)
    {
    ntst_timer_t now, tmp;
    ntst_timer_now(&now);
    tmp.tv_sec = now.tv_sec - timer->tv_sec;
    tmp.tv_nsec = now.tv_nsec - timer->tv_nsec;
    *result = TIMESPEC_TO_MS(tmp);
    *timer = now;
    return NTST_ERROR_OK;
    }

void ntst_timer_add_msec_ip(ntst_timer_t* timer, int val)
    {
    /* Could use / and %. But until C99 result was implementation-defined.
       The remainder is such that quot * y + rem == x*/
    const div_t dv = div(val, 1000);
    timer->tv_sec += dv.quot;
    if (dv.rem != 0)
        {
        timer->tv_nsec += dv.rem * 1000000L;
        if (timer->tv_nsec >= 1000000000L)
            {
            timer->tv_nsec -= 1000000000L;
            ++timer->tv_sec;
            }
        else if (timer->tv_nsec < 0)
            {
            timer->tv_nsec += 1000000000L;
            --timer->tv_sec;
            }
        }
    assert(timer->tv_nsec >= 0);
    assert(timer->tv_nsec < 1000000000L);
    }

ntst_error_t ntst_time_now(ntst_time_t* nt)
    {
    if (NTST_LIKELY(clock_gettime(CLOCK_REALTIME, nt) == 0))
        {
        return NTST_ERROR_OK;
        }
    return ntst_error_from_errno(errno);
    }
    
#endif


void ntst_time_zero_init(ntst_time_t* nt)
    {
    nt->tv_sec = 0;
    nt->tv_nsec = 0;
    }

void ntst_time_invalid_init(ntst_time_t* nt)
    {
    nt->tv_sec = -1;
    nt->tv_nsec = 0;
    }

int  ntst_time_is_valid(const ntst_time_t* nt)
    {
    if ((nt->tv_sec != -1) || (nt->tv_nsec != 0))
        return 1;
    return 0;
    }

ntst_error_t ntst_time_set_local(int year, int month, int day, int hour, int minute, int sec, long nsec, ntst_time_t* nt)
    {
    struct tm t;
    assert(nt != NULL);
    t.tm_year = year - 1900;
    t.tm_mon = month;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = sec;
    t.tm_isdst = -1;
    nt->tv_nsec = nsec;
    nt->tv_sec = mktime(&t);
    if (NTST_LIKELY(nt->tv_sec != -1))
        return NTST_ERROR_OK;
    return ntst_error_from_errno(errno);
    }

ntst_error_t ntst_time_set_gmt(int year, int month, int day, int hour, int minute, int sec, long nsec, ntst_time_t* nt)
    {
    struct tm t;
    assert(nt != NULL);
    t.tm_year = year - 1900;
    t.tm_mon = month;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = sec;
    t.tm_isdst = -1;
    nt->tv_nsec = nsec;
    nt->tv_sec = ntst_timegm(&t);
    if (NTST_LIKELY(nt->tv_sec != -1))
        return NTST_ERROR_OK;
    return ntst_error_from_errno(errno);
    }

void ntst_time_add_sec_ip(ntst_time_t* nt, int val)
    {
    nt->tv_sec += val;
    }

ntst_time_t ntst_time_add_sec(const ntst_time_t* nt, int val)
    {
    ntst_time_t ret;
    ret.tv_sec = nt->tv_sec + val;
    ret.tv_nsec = nt->tv_nsec;
    return ret;
    }

void ntst_time_from_time_t(time_t t, ntst_time_t* nt)
    {
    assert(nt != NULL);
    nt->tv_sec = t;
    nt->tv_nsec = 0;
    }

time_t ntst_time_to_time_t(const ntst_time_t* nt)
    {
    assert(nt != NULL);
    return nt->tv_sec;
    }

DotNetDateTime ntst_time_to_dotnettime(const ntst_time_t* nt)
    {
    /* long unixTimeStamp = (DateTime.Ticks - 621355968000000000) / 10000000; */
    long nano_tick;
    assert(nt != NULL);
    nano_tick = nt->tv_nsec / 100;
    assert(nano_tick >= 0);
    /* Наносекунды на секунды накладываются. Имхо не должно такого быть, но проверим на всякий. */
    assert(nano_tick < 10000000);
    return nt->tv_sec * (DotNetDateTime)10000000ull + (DotNetDateTime)621355968000000000ull + nano_tick;
    }

int64_t ntst_time_to_int64millisec(const ntst_time_t* nt)
    {
    assert(nt != NULL);
    return (nt->tv_sec * 1000ll) + (nt->tv_nsec / 1000000ll);
    }

void ntst_time_from_int64millisec(const int64_t* NTST_RESTRICT val, ntst_time_t* NTST_RESTRICT nt)
    {
    assert(nt != NULL);
    assert(val != NULL);
    /* Надо бы lldiv юзать, но только с C99. Нужно найти проверку */
    nt->tv_sec = *val / 1000;
    nt->tv_nsec = (*val % 1000) * 1000000ll;
    }

void ntst_time_from_gps_time(unsigned weeks, double seconds, ntst_time_t* nt)
    {
    /* Ticks between Unix epoch and GPS epoch http://stackoverflow.com/questions/20521750/ticks-between-unix-epoch-and-gps-epoch
    The correct ofset value is 315964800 seconds. */
    double seconds_integral;
    const double seconds_fraction = modf(seconds, &seconds_integral);
    assert(nt != NULL);
    nt->tv_sec = 315964800 + (weeks * 7 * 24 * 60 * 60) + (time_t)seconds_integral;
    nt->tv_nsec = (long)(seconds_fraction * 1000000000.0);
    }

unsigned ntst_time_to_gps_time(double* NTST_RESTRICT seconds, const ntst_time_t* NTST_RESTRICT nt)
    {
    unsigned weeks;
    time_t tmp = nt->tv_sec - 315964800;
    weeks = (unsigned)(tmp / (7*24*60*60));
    *seconds = (double)(tmp % (7*24*60*60));
    *seconds += nt->tv_nsec * 1000000000.0;
    return weeks;
    }

static void ntst_structtm_to_calendar(ntst_time_calendar_t* NTST_RESTRICT calendar, const struct tm* NTST_RESTRICT tm_struct)
    {
    calendar->year = tm_struct->tm_year + 1900;
    calendar->month = tm_struct->tm_mon;
    calendar->mday = tm_struct->tm_mday;
    calendar->hour = tm_struct->tm_hour;
    calendar->min = tm_struct->tm_min;
    calendar->sec = tm_struct->tm_sec;
    calendar->wday = tm_struct->tm_wday;
    }

static void ntst_structtm_from_calendar(const ntst_time_calendar_t* NTST_RESTRICT calendar, struct tm* NTST_RESTRICT tm_struct)
    {
    tm_struct->tm_year = calendar->year - 1900;
    tm_struct->tm_mon = calendar->month;
    tm_struct->tm_mday = calendar->mday;
    tm_struct->tm_hour = calendar->hour;
    tm_struct->tm_min = calendar->min;
    tm_struct->tm_sec = calendar->sec;
    tm_struct->tm_wday = calendar->wday;
    /* -1 == unknown */
    tm_struct->tm_isdst = -1;
    /* TODO not sure what to do. Transform calendar in time_t, then in struct tm? */
    tm_struct->tm_yday = 0;
    }

ntst_error_t ntst_time_to_calendar_local(ntst_time_calendar_t* NTST_RESTRICT calendar, const ntst_time_t* NTST_RESTRICT nt)
    {
    struct tm tm_struct;
    ntst_error_t ret;
    assert(calendar != NULL);
    assert(nt != NULL);
    ret = ntst_localtime(&nt->tv_sec, &tm_struct);
    if (NTST_LIKELY(ret == NTST_ERROR_OK))
        {
        ntst_structtm_to_calendar(calendar, &tm_struct);
        calendar->millisec = nt->tv_nsec / 1000000L;
        }
    return ret;
    }

ntst_error_t ntst_time_to_calendar_gmt(ntst_time_calendar_t* NTST_RESTRICT calendar, const ntst_time_t* NTST_RESTRICT nt)
    {
    struct tm tm_struct;
    ntst_error_t ret;
    assert(calendar != NULL);
    assert(nt != NULL);
    ret = ntst_gmtime(&nt->tv_sec, &tm_struct);
    if (NTST_LIKELY(ret == NTST_ERROR_OK))
        {
        ntst_structtm_to_calendar(calendar, &tm_struct);
        calendar->millisec = nt->tv_nsec / 1000000L;
        }
    return ret;
    }

ntst_error_t ntst_time_from_calendar_local(const ntst_time_calendar_t* NTST_RESTRICT calendar, ntst_time_t* NTST_RESTRICT nt)
    {
    struct tm tm_struct;
    assert(calendar != NULL);
    assert(nt != NULL);
    ntst_structtm_from_calendar(calendar, &tm_struct);
    nt->tv_sec = mktime(&tm_struct);
    if (NTST_LIKELY(nt->tv_sec != -1))
        {
        nt->tv_nsec = calendar->millisec * 1000000L;
        return NTST_ERROR_OK;
        }
    /* for mktime
     -1 if time cannot be represented as a time_t object
     (POSIX also requires EOVERFLOW to be stored in errno in this case)
    */
    return NTST_ERROR_EOVERFLOW;
    }

ntst_error_t ntst_time_from_calendar_gmt(const ntst_time_calendar_t* NTST_RESTRICT calendar, ntst_time_t* NTST_RESTRICT nt)
    {
    struct tm tm_struct;
    assert(calendar != NULL);
    assert(nt != NULL);
    ntst_structtm_from_calendar(calendar, &tm_struct);
    nt->tv_sec = ntst_timegm(&tm_struct);
    if (NTST_LIKELY(nt->tv_sec != -1))
        {
        nt->tv_nsec = calendar->millisec * 1000000L;
        return NTST_ERROR_OK;
        }
    /* for mktime
     -1 if time cannot be represented as a time_t object
     (POSIX also requires EOVERFLOW to be stored in errno in this case)
    */
    return NTST_ERROR_EOVERFLOW;
    }

int ntst_time_cmp(const ntst_time_t* a, const ntst_time_t* b)
    {
    return (a->tv_sec < b->tv_sec ? -1
      : a->tv_sec > b->tv_sec ? 1
      : a->tv_nsec < b->tv_nsec ? -1
      : a->tv_nsec > b->tv_nsec ? 1
      : 0);
    }

int ntst_print_calendar(char* NTST_RESTRICT buf, size_t buf_size, const struct ntst_time_calendar_t* NTST_RESTRICT calendar)
    {
    assert(buf != NULL);
    assert(calendar != NULL);
    return ntst_snprintf(buf, buf_size, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
                         (int)calendar->year,
                         (int)(calendar->month + 1),
                         (int)calendar->mday,
                         (int)calendar->hour,
                         (int)calendar->min,
                         (int)calendar->sec,
                         (int)calendar->millisec);
    }

int ntst_print_calendar_date(char* NTST_RESTRICT buf, size_t buf_size, const struct ntst_time_calendar_t* NTST_RESTRICT calendar)
    {
    assert(buf != NULL);
    assert(calendar != NULL);
    return ntst_snprintf(buf, buf_size, "%04d-%02d-%02d",
                         (int)calendar->year,
                         (int)(calendar->month + 1),
                         (int)calendar->mday);
    }

int ntst_print_calendar_time(char* NTST_RESTRICT buf, size_t buf_size, const struct ntst_time_calendar_t* NTST_RESTRICT calendar)
    {
    assert(buf != NULL);
    assert(calendar != NULL);
    return ntst_snprintf(buf, buf_size, "%02d:%02d:%02d.%03d",
                         (int)calendar->hour,
                         (int)calendar->min,
                         (int)calendar->sec,
                         (int)calendar->millisec);
    }

double ntst_time_to_double_time_t(const ntst_time_t* t)
    {
    return t->tv_sec + (t->tv_nsec / 1000000000.0);
    }

ntst_error_t ntst_calendar_now_local(ntst_time_calendar_t* cal)
    {
    ntst_time_t now;
    const ntst_error_t er = ntst_time_now(&now);
    if (NTST_LIKELY(er == NTST_ERROR_OK))
        {
        return ntst_time_to_calendar_local(cal, &now);
        }
    return er;
    }

ntst_error_t ntst_calendar_now_gmt(ntst_time_calendar_t* cal)
    {
    ntst_time_t now;
    const ntst_error_t er = ntst_time_now(&now);
    if (NTST_LIKELY(er == NTST_ERROR_OK))
        {
        return ntst_time_to_calendar_gmt(cal, &now);
        }
    return er;
    }
