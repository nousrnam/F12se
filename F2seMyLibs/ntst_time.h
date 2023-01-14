#ifndef GNTST_TIME_H
#define GNTST_TIME_H

#include <time.h>
#include <stdint.h>
#include "ntst_base.h"
#include "ntst_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t DotNetDateTime; /* Формат времени в протоколе скаута. */
/*ntst_static_assert(sizeof(DotNetDateTime) == 8, "Invalid NetDateTime size!");*/
/* long unixTimeStamp = (DateTime.Ticks - 621355968000000000) / 10000000; */

ntst_error_t ntst_localtime(const time_t* NTST_RESTRICT time, struct tm* NTST_RESTRICT tm_struct) NTST_NONNULL_ARGS;
ntst_error_t ntst_gmtime(const time_t* NTST_RESTRICT time, struct tm* NTST_RESTRICT tm_struct) NTST_NONNULL_ARGS;
/* -1 on error */
time_t ntst_timegm(struct tm* tm_struct) NTST_NONNULL_ARGS;

#if defined(_WIN32)
#include <windows.h>
void ntst_win_timet_to_filetime(time_t t, LPFILETIME pft) NTST_NONNULL_ARGS;
void ntst_win_filetime_to_timet(LPFILETIME pft, time_t* t) NTST_NONNULL_ARGS;
typedef DWORD ntst_timer_t;
typedef struct ntst_time_t
    {
    time_t tv_sec;  /* Seconds */
    long   tv_nsec; /* Nanoseconds */
    } ntst_time_t;
ntst_error_t ntst_win_systemtime_to_ntsttime(SYSTEMTIME* NTST_RESTRICT st, ntst_time_t* NTST_RESTRICT t) NTST_NONNULL_ARGS;
ntst_error_t ntst_win_oletime_to_ntsttime(double time, ntst_time_t* NTST_RESTRICT t) NTST_NONNULL_ARGS;
#else
typedef struct timespec ntst_timer_t;
typedef struct timespec ntst_time_t;
#endif

typedef struct ntst_time_calendar_t
    {
    int year;
    int month; /* months since January – [0, 11] */
    int mday; /* day of the month – [1, 31] */
    int hour; /* hours since midnight – [0, 23] */
    int min; /* minutes after the hour – [0, 59] */
    int sec; /* seconds after the minute – [0, 60] */
    int millisec; /* milliseconds after the second – [0, 999] */
    int wday; /* days since Sunday – [0, 6] */
    } ntst_time_calendar_t;

/* For now ntst_timer_now always successful */
ntst_error_t ntst_timer_now(ntst_timer_t* timer) NTST_NONNULL_ARGS;
void ntst_timer_set_long_ago(ntst_timer_t* timer) NTST_NONNULL_ARGS;
int ntst_timer_diff_ms(const ntst_timer_t* NTST_RESTRICT timer_end, const ntst_timer_t* NTST_RESTRICT timer_begin) NTST_NONNULL_ARGS;
ntst_error_t ntst_timer_elapsed_ms(const ntst_timer_t* NTST_RESTRICT timer, unsigned* NTST_RESTRICT result) NTST_NONNULL_ARGS;
ntst_error_t ntst_timer_elapsed_ms_set_now(ntst_timer_t* NTST_RESTRICT timer, unsigned* NTST_RESTRICT result) NTST_NONNULL_ARGS;
void ntst_timer_add_msec_ip(ntst_timer_t* timer, int val) NTST_NONNULL_ARGS;

void ntst_time_zero_init(ntst_time_t* nt) NTST_NONNULL_ARGS;
void ntst_time_invalid_init(ntst_time_t* nt) NTST_NONNULL_ARGS;
int  ntst_time_is_valid(const ntst_time_t* nt) NTST_NONNULL_ARGS;
ntst_error_t ntst_time_now(ntst_time_t* nt) NTST_NONNULL_ARGS;
/* month [0, 11], day [1, 31], hour [0, 23], minute [0, 59], sec [0, 60] */
ntst_error_t ntst_time_set_local(int year, int month, int day, int hour, int minute, int sec, long nsec, ntst_time_t* nt) NTST_NONNULL_ARGS;
ntst_error_t ntst_time_set_gmt(int year, int month, int day, int hour, int minute, int sec, long nsec, ntst_time_t* nt) NTST_NONNULL_ARGS;
void ntst_time_add_sec_ip(ntst_time_t* nt, int val) NTST_NONNULL_ARGS;
ntst_time_t ntst_time_add_sec(const ntst_time_t* nt, int val) NTST_NONNULL_ARGS;

void ntst_time_from_time_t(time_t t, ntst_time_t* nt) NTST_NONNULL_ARGS;
time_t ntst_time_to_time_t(const ntst_time_t* nt) NTST_NONNULL_ARGS;

DotNetDateTime ntst_time_to_dotnettime(const ntst_time_t* nt) NTST_NONNULL_ARGS;
int64_t ntst_time_to_int64millisec(const ntst_time_t* nt) NTST_NONNULL_ARGS;
void ntst_time_from_int64millisec(const int64_t* NTST_RESTRICT val, ntst_time_t* NTST_RESTRICT nt) NTST_NONNULL_ARGS;

void ntst_time_from_gps_time(unsigned weeks, double seconds, ntst_time_t* nt) NTST_NONNULL_ARGS;
unsigned ntst_time_to_gps_time(double* NTST_RESTRICT seconds, const ntst_time_t* NTST_RESTRICT nt) NTST_NONNULL_ARGS;

ntst_error_t ntst_time_to_calendar_local(ntst_time_calendar_t* NTST_RESTRICT calendar, const ntst_time_t* NTST_RESTRICT nt) NTST_NONNULL_ARGS;
ntst_error_t ntst_time_to_calendar_gmt(ntst_time_calendar_t* NTST_RESTRICT calendar, const ntst_time_t* NTST_RESTRICT nt) NTST_NONNULL_ARGS;

ntst_error_t ntst_time_from_calendar_local(const ntst_time_calendar_t* NTST_RESTRICT calendar, ntst_time_t* NTST_RESTRICT nt) NTST_NONNULL_ARGS;
ntst_error_t ntst_time_from_calendar_gmt(const ntst_time_calendar_t* NTST_RESTRICT calendar, ntst_time_t* NTST_RESTRICT nt) NTST_NONNULL_ARGS;

int ntst_time_cmp(const ntst_time_t* a, const ntst_time_t* b);

int ntst_print_calendar(char* NTST_RESTRICT buf, size_t buf_size, const struct ntst_time_calendar_t* NTST_RESTRICT calendar) NTST_NONNULL_ARGS;
int ntst_print_calendar_date(char* NTST_RESTRICT buf, size_t buf_size, const struct ntst_time_calendar_t* NTST_RESTRICT calendar) NTST_NONNULL_ARGS;
int ntst_print_calendar_time(char* NTST_RESTRICT buf, size_t buf_size, const struct ntst_time_calendar_t* NTST_RESTRICT calendar) NTST_NONNULL_ARGS;

double ntst_time_to_double_time_t(const ntst_time_t* t);

ntst_error_t ntst_calendar_now_local(ntst_time_calendar_t* cal) NTST_NONNULL_ARGS;
ntst_error_t ntst_calendar_now_gmt(ntst_time_calendar_t* cal) NTST_NONNULL_ARGS;

#ifdef __cplusplus
}
inline bool operator<(const ntst_time_t& a, const ntst_time_t& b) {return ntst_time_cmp(&a, &b) < 0;}
inline bool operator>(const ntst_time_t& a, const ntst_time_t& b) {return ntst_time_cmp(&a, &b) > 0;}
inline bool operator==(const ntst_time_t& a, const ntst_time_t& b) {return ntst_time_cmp(&a, &b) == 0;}
inline bool operator!=(const ntst_time_t& a, const ntst_time_t& b) {return ntst_time_cmp(&a, &b) != 0;}
inline bool operator<=(const ntst_time_t& a, const ntst_time_t& b) {return ntst_time_cmp(&a, &b) <= 0;}
inline bool operator>=(const ntst_time_t& a, const ntst_time_t& b) {return ntst_time_cmp(&a, &b) >= 0;}
#endif

#endif
