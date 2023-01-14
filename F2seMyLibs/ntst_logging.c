#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h> /* malloc */

#include "ntst_logging.h"
#include "ntst_string.h"
#include "ntst_mem.h"

int logmode = NTST_LOGM_ALL;
static ntst_log_callback_type callback = NULL;
static void* data = NULL;

void ntst_log_set_mode(int mode)
    {
    assert(mode == (mode & NTST_LOGM_ALL));
    logmode = mode;
    }

const char* ntst_log_mode_str(ntst_log_modes mode, size_t* str_len)
    {
    assert(str_len != NULL);
    switch (mode)
        {
        case NTST_LOGM_INFO:
            *str_len = NTST_STR_LEN("INFO");
            return "INFO";
        case NTST_LOGM_WARNING:
            *str_len = NTST_STR_LEN("WARNING");
            return "WARNING";
        case NTST_LOGM_ERROR:
            *str_len = NTST_STR_LEN("ERROR");
            return "ERROR";
        }
    *str_len = NTST_STR_LEN("INVALID");
    return "INVALID";
    }

void ntst_log_common(const char* str, size_t str_len, ntst_log_modes prefix_num)
    {
    assert(str != NULL);
    if (NTST_LIKELY(callback != NULL))
        {
        ntst_time_t nt;
        ntst_time_now(&nt);
        callback(str, str_len, prefix_num, &nt, data);
        }
    }

void ntst_log_info(const char* str)
    {
    if (logmode & NTST_LOGM_INFO)
        ntst_log_common(str, strlen(str), NTST_LOGM_INFO);
    }

void ntst_log_warning(const char* str)
    {
    if (logmode & NTST_LOGM_WARNING)
        ntst_log_common(str, strlen(str), NTST_LOGM_WARNING);
    }

void ntst_log_error(const char* str)
    {
    if (logmode & NTST_LOGM_ERROR)
        ntst_log_common(str, strlen(str), NTST_LOGM_ERROR);
    }

void ntst_log_blob(const char* blob_name, ntst_log_modes mode, const void* blob, size_t blob_size)
    {
    char* str = ntst_blob_to_str(blob_name, blob, &blob_size);
    if (NTST_LIKELY(str != NULL))
        {
        ntst_log_common(str, blob_size, mode);
        NTST_FREE(str);
        }
    else
        {
        ntst_log_common("Can't allocate memory for blob.", NTST_STR_LEN("Can't allocate memory for blob."), NTST_LOGM_ERROR);
        }
    }

#if defined(_WIN32)

void ntst_log_common_w(const wchar_t* str, size_t wstr_len, ntst_log_modes prefix_num)
    {
    char* tmp_str;
    /*
    WideCharToMultiByte does not null-terminate an output string if the input string length is explicitly specified without a terminating null character. To null-terminate an output string for this function, the application should pass in -1 or explicitly count the terminating null character for the input string.
    */
    int size_need = WideCharToMultiByte(CP_UTF8, 0, str, wstr_len, NULL, 0, NULL, NULL);
    assert(size_need > 0);
    tmp_str = (char*)NTST_MALLOC(size_need + 1); /* For null terminator */
    if (NTST_LIKELY(tmp_str != NULL))
        {
        WideCharToMultiByte(CP_UTF8, 0, str, wstr_len, tmp_str, size_need, NULL, NULL);
        tmp_str[size_need] = '\0';
        ntst_log_common(tmp_str, size_need, prefix_num);
        NTST_FREE(tmp_str);
        }
    }

void ntst_log_info_w(const wchar_t* str)
    {
    if (logmode & NTST_LOGM_INFO)
        ntst_log_common_w(str, wcslen(str), NTST_LOGM_INFO);
    }

void ntst_log_warning_w(const wchar_t* str)
    {
    if (logmode & NTST_LOGM_WARNING)
        ntst_log_common_w(str, wcslen(str), NTST_LOGM_WARNING);
    }

void ntst_log_error_w(const wchar_t* str)
    {
    if (logmode & NTST_LOGM_ERROR)
        ntst_log_common_w(str, wcslen(str), NTST_LOGM_ERROR);
    }

#endif

ntst_log_callback_type ntst_log_set_callback(ntst_log_callback_type cb, void* userData)
    {
    ntst_log_callback_type oldCallback = callback;
    callback = cb;
    data = userData;
    return oldCallback;
    }
