#ifndef GNTST_LOGGING_H
#define GNTST_LOGGING_H

/* #define NTST_LOG_FILEPATH "log.txt" */

#include "ntst_base.h"
#include "ntst_time.h"

#if defined(ENTST_ENABLE_INNER_LOGGING)
#ifdef NTST_LOG_REPLACE_HEADER
#define NTST_QUOTEARG(x) NTST_QUOTEARG1(x)
#define NTST_QUOTEARG1(x) #x
#include NTST_QUOTEARG(NTST_LOG_REPLACE_HEADER)
#undef NTST_QUOTEARG
#undef NTST_QUOTEARG1
#endif
#ifndef NTST_LOG_INFO
#define NTST_LOG_INFO ntst_log_info
#endif
#ifndef NTST_LOG_WARNING
#define NTST_LOG_WARNING ntst_log_warning
#endif
#ifndef NTST_LOG_ERROR
#define NTST_LOG_ERROR ntst_log_error
#endif
#ifndef NTST_LOG_BLOB
#define NTST_LOG_BLOB ntst_log_blob
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ntst_log_modes
    {
    NTST_LOGM_INFO = 1,
    NTST_LOGM_WARNING = 2,
    NTST_LOGM_ERROR = 4
    } ntst_log_modes;

#define NTST_LOGM_ALL (NTST_LOGM_INFO | NTST_LOGM_WARNING | NTST_LOGM_ERROR)

void ntst_log_set_mode(int mode);

const char* ntst_log_mode_str(ntst_log_modes mode, size_t* str_len) NTST_NONNULL_ARGS;

void ntst_log_info(const char* str) NTST_NONNULL_ARGS;
void ntst_log_warning(const char* str) NTST_NONNULL_ARGS;
void ntst_log_error(const char* str) NTST_NONNULL_ARGS;
void ntst_log_blob(const char* blob_name, ntst_log_modes mode, const void* blob, size_t blob_size) NTST_NONNULL_ARGS;

#if defined(_WIN32)
void ntst_log_info_w(const wchar_t* str) NTST_NONNULL_ARGS;
void ntst_log_warning_w(const wchar_t* str) NTST_NONNULL_ARGS;
void ntst_log_error_w(const wchar_t* str) NTST_NONNULL_ARGS;
#endif

typedef void (*ntst_log_callback_type)(const char* str, size_t str_len, ntst_log_modes mode, const ntst_time_t* nt, void* userData);
ntst_log_callback_type ntst_log_set_callback(ntst_log_callback_type cb, void* userData);

#ifdef __cplusplus
}
#endif

#endif
