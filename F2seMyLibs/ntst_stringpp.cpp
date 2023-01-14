#include <algorithm>
#include <cassert>

#include "ntst_stringpp.hpp"

namespace ntst
{

class TmpMemBuffer
    {
public:
    TmpMemBuffer(size_t s) {data = new char[s];}
    ~TmpMemBuffer() {delete[] data;}
    char* GetData() NTST_NOEXCEPT {return data;}
#if defined(NTST_HAS_RVALUE_REF)
    TmpMemBuffer(TmpMemBuffer&& b) NTST_NOEXCEPT
        {
        data = b.data;
        }
    TmpMemBuffer& operator=(TmpMemBuffer&& b) NTST_NOEXCEPT
        {
        std::swap(data, b.data);
        return *this;
        }
#endif
private:
    TmpMemBuffer(const TmpMemBuffer&);
    TmpMemBuffer& operator=(const TmpMemBuffer&);
    char* data;
    };

#define STACK_BUFFER_SIZE 128
std::string string_vprintf(const char* format, va_list valist)
    {
    std::string result;
    char stack_buffer[STACK_BUFFER_SIZE];
    va_list valist_copy;
#if defined(_MSC_VER) && (_MSC_VER < 1800) /* 12.0 (2013) */
    /* old msvc doesn't have va_copy. Only since Visual Studio 2013 */
    valist_copy = valist;
#else
    va_copy(valist_copy, valist);
#endif
    const int would_be_printed = ntst_vsnprintf(stack_buffer, STACK_BUFFER_SIZE, format, valist);
    if (would_be_printed >= STACK_BUFFER_SIZE)
        {
        const size_t need = would_be_printed + 1; // +1 for '\0'
        TmpMemBuffer buf(need);
        ntst_vsnprintf(buf.GetData(), need, format, valist_copy);
        result.assign(buf.GetData(), would_be_printed);
        }
    else
        {
        if (NTST_LIKELY(would_be_printed >= 0))
            {
            result.assign(stack_buffer, would_be_printed);
            }
        else
            {
            result = "printf error!";
            }
        }
    va_end(valist_copy);
    return result;
    }

std::string string_printf(const char* format, ...)
    {
    va_list valist;
    va_start(valist, format);
    std::string result = string_vprintf(format, valist);
    va_end(valist);
    return result;
    }

void ascii_string_to_lower_inplace(std::string& str)
    {
    std::transform(str.begin(), str.end(), str.begin(), ntst_ascii_tolower);
    }

static bool char_in_trim(char c, const char* trim, unsigned trim_len)
    {
    for (unsigned i = 0; i < trim_len; ++i)
        {
        if (trim[i] == c)
            return true;
        }
    return false;
    }

void trim_string(std::string& str)
    {
    const std::string::size_type str_len = str.size();
    if (str_len == 0)
        return;
    std::string::size_type offset_start, offset_end;
    for (offset_start = 0; offset_start < str_len; ++offset_start)
        if (!NTST_ISSPACE(str[offset_start]))
            break;
    for (offset_end = str_len; offset_end > offset_start; --offset_end)
        if (!NTST_ISSPACE(str[offset_end - 1]))
            break;
    if ((offset_start != 0) || (offset_end != str_len))
        str.assign(str, offset_start, offset_end - offset_start);
    }

void trim_string(std::string& str, const char* trim)
    {
    assert(trim);
    const size_t trim_len = strlen(trim);
    const std::string::size_type str_len = str.size();
    if (str_len == 0)
        return;
    std::string::size_type offset_start, offset_end;
    for (offset_start = 0; offset_start < str_len; ++offset_start)
        if (!char_in_trim(str[offset_start], trim, trim_len))
            break;
    for (offset_end = str_len; offset_end > offset_start; --offset_end)
        if (!char_in_trim(str[offset_end - 1], trim, trim_len))
            break;
    if ((offset_start != 0) || (offset_end != str_len))
        str.assign(str, offset_start, offset_end - offset_start);
    }

void trim_string_left(std::string& str)
    {
    const std::string::size_type str_len = str.size();
    std::string::size_type erase_num;
    for (erase_num = 0; erase_num < str_len; ++erase_num)
        if (!NTST_ISSPACE(str[erase_num]))
            break;
    str.erase(0, erase_num);
    }

void trim_string_left(std::string& str, const char* trim)
    {
    assert(trim);
    const size_t trim_len = strlen(trim);
    const std::string::size_type str_len = str.size();
    std::string::size_type erase_num;
    for (erase_num = 0; erase_num < str_len; ++erase_num)
        if (!char_in_trim(str[erase_num], trim, trim_len))
            break;
    str.erase(0, erase_num);
    }

void trim_string_right(std::string& str)
    {
    std::string::size_type new_size;
    for (new_size = str.size(); new_size > 0; --new_size)
        if (!NTST_ISSPACE(str[new_size-1]))
            break;
    str.resize(new_size);
    }

void trim_string_right(std::string& str, const char* trim)
    {
    assert(trim);
    const size_t trim_len = strlen(trim);
    std::string::size_type new_size;
    for (new_size = str.size(); new_size > 0; --new_size)
        if (!char_in_trim(str[new_size-1], trim, trim_len))
            break;
    str.resize(new_size);
    }

void trim_string_trailing_decimal_zeros(std::string& str)
    {
    std::string::size_type cur_pos = str.size();
    std::string::size_type first_trailing_zero_pos = cur_pos;
    while (cur_pos > 0)
        {
        --cur_pos;
        const char c = str[cur_pos];
        if (c == '0')
            {
            if (first_trailing_zero_pos == cur_pos + 1)
                first_trailing_zero_pos = cur_pos;
            }
        else if (c == '.' || c == ',')
            {
            std::string::size_type new_size;
            if (first_trailing_zero_pos != cur_pos + 1)
                new_size = first_trailing_zero_pos;
            else
                new_size = cur_pos;
            str.resize(new_size);
            break;
            }
        else if (!isdigit((unsigned char)c))
            break;
        }
    }

} // namespace ntst
