#ifndef GNTST_LOGGING_HPP
#define GNTST_LOGGING_HPP

#include "ntst_logging.h"

#include <string>

void ntst_log_info(const std::string& str);
void ntst_log_warning(const std::string& str);
void ntst_log_error(const std::string& str);

void ntst_log_blob(const std::string& blob_name, ntst_log_modes mode, const void* blob, size_t blob_size);

#ifdef ENTST_ENABLE_BOOST_USE
#include <boost/system/error_code.hpp>

#include "ntst_stringpp.hpp"

#if defined(_WIN32)
#include <assert.h>
#include "ntst_mem.h"
#include "unicode/ntst_unicode_conv.h"
#endif

template<typename StrType>
void ntst_log_error_boost(const StrType& str, const boost::system::error_code& ec)
    {
    try {
        std::string text = str;
        text += (ntst::to_string(ec.value()) + ' ');
#if defined(_WIN32)
        std::string boost_msg = ec.message();
        size_t tmp_buf_size = boost_msg.size() * NTST_UTF8_MAX_CP_LEN;
        size_t tmp_buf_size_used = tmp_buf_size;
        NTST_ARRAY_ON_STACK(char, tmp_buf, tmp_buf_size);
        size_t consumed = ntst_localstr_to_utf8(boost_msg.c_str(), boost_msg.size(), tmp_buf, &tmp_buf_size_used);
        assert(consumed == boost_msg.size());
        (void)consumed;
        if (tmp_buf_size_used <= tmp_buf_size)
            text.append(tmp_buf, tmp_buf_size_used);
        else
            text += boost_msg;
#else
        text += ec.message();
#endif

        NTST_LOG_ERROR(text);
        }
    catch (...)
        {
        NTST_LOG_ERROR("ntst_log_error_boost exception");
        }
    }
#endif

std::string ntst_log_param_to_str(const char* str, size_t str_len, ntst_log_modes mode, const ntst_time_t* nt, bool time_only = false);

#if defined(_WIN32)
void ntst_log_info_w(const std::wstring& str);
void ntst_log_warning_w(const std::wstring& str);
void ntst_log_error_w(const std::wstring& str);
#endif

#ifdef ENTST_ENABLE_QT_USE
class QString;
void ntst_log_info(const QString& str);
void ntst_log_warning(const QString& str);
void ntst_log_error(const QString& str);
#endif

#ifdef ENTST_ENABLE_WX_USE
class wxString;
void ntst_log_info(const wxString& str);
void ntst_log_warning(const wxString& str);
void ntst_log_error(const wxString& str);
#endif

#endif
