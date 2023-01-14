#include "ntst_loggingpp.hpp"

extern "C" {
extern int logmode;
void ntst_log_common(const char* str, size_t str_len, ntst_log_modes prefix_num);
void ntst_log_common_w(const wchar_t* str, size_t wstr_len, ntst_log_modes prefix_num);
}

void ntst_log_info(const std::string& str)
    {
    if (logmode & NTST_LOGM_INFO)
        ntst_log_common(str.c_str(), str.size(), NTST_LOGM_INFO);
    }

void ntst_log_warning(const std::string& str)
    {
    if (logmode & NTST_LOGM_WARNING)
        ntst_log_common(str.c_str(), str.size(), NTST_LOGM_WARNING);
    }

void ntst_log_error(const std::string& str)
    {
    if (logmode & NTST_LOGM_ERROR)
        ntst_log_common(str.c_str(), str.size(), NTST_LOGM_ERROR);
    }

void ntst_log_blob(const std::string& blob_name, ntst_log_modes mode, const void* blob, size_t blob_size)
    {
    ntst_log_blob(blob_name.c_str(), mode, blob, blob_size);
    }

std::string ntst_log_param_to_str(const char* str, size_t str_len, ntst_log_modes mode, const ntst_time_t* nt, bool time_only)
    {
    char datetime_buf[40];
    const char* datetime_ptr;
    int datetime_len;
    ntst_time_calendar_t calendar;
    ntst_time_to_calendar_local(&calendar, nt);
    if (time_only)
        datetime_len = ntst_print_calendar_time(datetime_buf, sizeof(datetime_buf) - 1, &calendar);
    else
        datetime_len = ntst_print_calendar(datetime_buf, sizeof(datetime_buf) - 1, &calendar);
    if (NTST_LIKELY(datetime_len > 0))
        {
        datetime_buf[datetime_len] = ' ';
        ++datetime_len;
        datetime_ptr = datetime_buf;
        }
    else
        {
        datetime_ptr = "TIME_ERROR ";
        datetime_len = NTST_STR_LEN("TIME_ERROR ");
        }
    std::string result = std::string(datetime_ptr, datetime_len);
    if (NTST_UNLIKELY(mode != NTST_LOGM_INFO))
        {
        size_t mode_len;
        const char* mode_str = ntst_log_mode_str(mode, &mode_len);
        // 2 == ' ' + '\0'
        result.reserve(datetime_len + mode_len + str_len + 2);
        result.append(mode_str, mode_len);
        result += ' ';
        }
    result.append(str, str_len);
    return result;
    }

#if defined(_WIN32)
void ntst_log_info_w(const std::wstring& str)
    {
    if (logmode & NTST_LOGM_INFO)
        ntst_log_common_w(str.c_str(), str.size(), NTST_LOGM_INFO);
    }

void ntst_log_warning_w(const std::wstring& str)
    {
    if (logmode & NTST_LOGM_WARNING)
        ntst_log_common_w(str.c_str(), str.size(), NTST_LOGM_WARNING);
    }

void ntst_log_error_w(const std::wstring& str)
    {
    if (logmode & NTST_LOGM_ERROR)
        ntst_log_common_w(str.c_str(), str.size(), NTST_LOGM_ERROR);
    }

#endif

#ifdef ENTST_ENABLE_QT_USE
#include <QString>
#include "ntst_stringpp.hpp"
void ntst_log_info(const QString& str)
    {
    ntst_log_info(QStringToStdUtf8(str));
    }
void ntst_log_warning(const QString& str)
    {
    ntst_log_warning(QStringToStdUtf8(str));
    }
void ntst_log_error(const QString& str)
    {
    ntst_log_error(QStringToStdUtf8(str));
    }
#endif // ENTST_ENABLE_QT_USE

#ifdef ENTST_ENABLE_WX_USE
#include "ntst_stringpp.hpp"
void ntst_log_info(const wxString& str)
    {
    ntst_log_info(WxStringToStdUtf8(str));
    }
void ntst_log_warning(const wxString& str)
    {
    ntst_log_warning(WxStringToStdUtf8(str));
    }
void ntst_log_error(const wxString& str)
    {
    ntst_log_error(WxStringToStdUtf8(str));
    }
#endif // ENTST_ENABLE_WX_USE
