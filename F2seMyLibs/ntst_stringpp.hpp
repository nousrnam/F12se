#ifndef GNTST_STRING_HPP
#define GNTST_STRING_HPP

#include <string>
#include <sstream>
#include <limits> // std::numeric_limits
#include <climits> // UCHAR_MAX

#include "ntst_string.h"

namespace ntst
{

template <class ContainerT>
void tokenize(const std::string& str, ContainerT& tokens,
              const std::string& delimiters = " \t", bool trimEmpty = false)
    {
    typedef typename ContainerT::value_type ValueType;
    typedef typename ValueType::size_type SizeType;
    SizeType pos, lastPos = 0;
    while (true)
        {
        pos = str.find_first_of(delimiters, lastPos);
        if (pos != std::string::npos)
            {
            if (pos != lastPos || !trimEmpty)
                tokens.push_back(ValueType(str.data() + lastPos, pos - lastPos));
            }
        else
            break;
        lastPos = pos + 1;
        }
    pos = str.size();
    if (pos != lastPos || !trimEmpty)
        tokens.push_back(ValueType(str.data() + lastPos, pos - lastPos));
    }

inline bool strhasprefix(const std::string& string, const std::string& prefix)
    {
    if (string.size() < prefix.size())
        return false;
    return memcmp(string.c_str(), prefix.c_str(), prefix.size()) == 0;
    }

template <size_t arr_size>
bool strhasprefix(const char* string, char const (&prefix)[arr_size])
    {
    return (strncmp(string, prefix, arr_size - NTST_ONE_NULL_TERM) == 0);
    }

template <size_t arr_size>
const char* strnstr(const char* string, char const (&substr)[arr_size], unsigned n)
    {
    for (unsigned i = 0; i <= n; ++i)
        {
        if (strncmp(string, substr, arr_size - NTST_ONE_NULL_TERM) == 0)
            return string;
        if (NTST_UNLIKELY(*string == '\0'))
            return NULL;
        ++string;
        }
    return NULL;
    }

/* CharPtr == "char*" || "const char*" */
template <class CharPtr, size_t ArrS>
CharPtr strstr_after(CharPtr str, char const (&substr)[ArrS])
    {
    CharPtr result = strstr(str, substr);
    if (result != NULL)
        return result + ArrS - NTST_ONE_NULL_TERM;
    return NULL;
    }

template <typename StrT>
bool str_offset_to_first_not_space(const StrT& str, size_t& offset)
    {
    const size_t str_size = str.size();
    for (size_t i = offset; i < str_size; ++i)
        {
        if (!NTST_ISSPACE(str[i]))
            {
            offset = i;
            return true;
            }
        }
    return false;
    }

inline size_t str_to_val(const char* str, int* result) {return ntst_str10toi(str, result);}
inline size_t str_to_val(const std::string& str, int* result) {return str_to_val(str.c_str(), result);}
inline size_t str_to_val(const char* str, unsigned* result) {return ntst_str10tou(str, result);}
inline size_t str_to_val(const std::string& str, unsigned* result) {return str_to_val(str.c_str(), result);}
inline size_t str_to_val(const char* str, double* result) {return ntst_strtod(str, result);}
inline size_t str_to_val(const std::string& str, double* result) {return str_to_val(str.c_str(), result);}

std::string string_vprintf(const char* format, va_list arg);
std::string string_printf(const char* format, ...)
NTST_ATTRIB_PRINTF_FMT(1,2)
;

void ascii_string_to_lower_inplace(std::string& str);

void trim_string(std::string& str);
void trim_string(std::string& str, const char* trim);
void trim_string_left(std::string& str);
void trim_string_left(std::string& str, const char* trim);
void trim_string_right(std::string& str);
void trim_string_right(std::string& str, const char* trim);
void trim_string_trailing_decimal_zeros(std::string& str);

namespace hidden
{
template <typename Tstr, typename Tval>
void insert_not_as_char(Tstr& ss, Tval val, bool hex)
    {
    // not only char, but int8_t, etc are also transformed into characters
    if (sizeof(Tval) == 1 && std::numeric_limits<Tval>::is_integer)
        {
        if (std::numeric_limits<Tval>::is_signed)
            {
            int v = static_cast<int>(val);
            if (hex) // prevent negative values xx turning into ffffffxx
                v &= UCHAR_MAX;
            ss << v;
            }
        else
            ss << static_cast<unsigned>(val);
        }
    else
        ss << val;
    }
}

template <typename T>
std::string to_string_hex(T val)
    {
    std::ostringstream ss(std::ios_base::out);
    ss << std::hex;
    hidden::insert_not_as_char(ss, val, true);
    return ss.str();
    }

template <typename T>
std::string to_string_hex_pref(T val)
    {
    std::ostringstream ss(std::ios_base::out);
    ss << std::hex << "0x";
    hidden::insert_not_as_char(ss, val, true);
    return ss.str();
    }

template <typename T>
std::wstring to_wstring_hex(T val)
    {
    std::wostringstream ss(std::ios_base::out);
    ss << std::hex;
    hidden::insert_not_as_char(ss, val, true);
    return ss.str();
    }

template <typename T>
std::wstring to_wstring_hex_pref(T val)
    {
    std::wostringstream ss(std::ios_base::out);
    ss << std::hex << L"0x";
    hidden::insert_not_as_char(ss, val, true);
    return ss.str();
    }

#if (defined(__cplusplus) && (__cplusplus >= 201103L)) \
    || (defined(_MSC_VER) && (_MSC_VER >= 1900))
    // Not sure if _MSC_VER check is correct. to_string was bugged when appeared "ambiguous call to overloaded function". Using it with VS2015, but maybe it was already good before.
#include <type_traits>
template <typename T>
std::string to_string(T val, typename std::enable_if<!std::is_pointer<T>::value>::type* = 0)
    {
    return std::to_string(val);
    }
// For pointers
template <typename T>
std::string to_string(T val, typename std::enable_if<std::is_pointer<T>::value>::type* = 0)
    {
    return to_string_hex_pref(val);
    }
#if defined(_GLIBCXX_HAVE_BROKEN_VSWPRINTF)
#define NTST_NEED_STREAM_TO_WSTRING
#else
template <typename T>
std::wstring to_wstring(T val, typename std::enable_if<!std::is_pointer<T>::value>::type* = 0)
    {
    return std::to_wstring(val);
    }
template <typename T>
std::wstring to_wstring(T val, typename std::enable_if<std::is_pointer<T>::value>::type* = 0)
    {
    return to_wstring_hex_pref(val);
    }
#endif
#else
template <typename T>
std::string to_string(T val)
    {
    // Constructor call to pacify cppcheck
    std::ostringstream ss(std::ios_base::out);
    hidden::insert_not_as_char(ss, val, false);
    return ss.str();
    }
#define NTST_NEED_STREAM_TO_WSTRING
#endif

#if defined(NTST_NEED_STREAM_TO_WSTRING)
template <typename T>
std::wstring to_wstring(T val)
    {
    std::wostringstream ss(std::ios_base::out);
    hidden::insert_not_as_char(ss, val, false);
    return ss.str();
    }
#undef NTST_NEED_STREAM_TO_WSTRING
#endif

#if ((!defined(_MSC_VER) && (__cplusplus >= 201103L)) || (_MSC_VER >= 1900))
// visual studio 2010 does not like suffixes[arr_size] default values. 2015 is fine.
// IEC standard - KiB. SI standard - kB(small k)
template <size_t arr_size>
int pretty_print_size1000(long long bytes, char* NTST_RESTRICT buff, unsigned buff_len, const char* NTST_RESTRICT format = "%.2f %s", const char* NTST_RESTRICT suffixes[arr_size] = {"B", "kB", "MB", "TB", "PB"})
    {
    return ntst_pretty_print_size1000(bytes, buff, buff_len, format, suffixes, arr_size);
    }
template <size_t arr_size>
int pretty_print_size1024(long long bytes, char* NTST_RESTRICT buff, unsigned buff_len, const char* NTST_RESTRICT format = "%.2f %s", const char* NTST_RESTRICT suffixes[arr_size] = {"B", "KiB", "MiB", "TiB", "PiB"})
    {
    return ntst_pretty_print_size1024(bytes, buff, buff_len, format, suffixes, arr_size);
    }

template <size_t arr_size>
std::string size_to_pretty1000(long long bytes, const char* NTST_RESTRICT format = "%.2f %s", const char* NTST_RESTRICT suffixes[arr_size] = {"B", "kB", "MB", "TB", "PB"})
    {
    unsigned suf;
    double val = ntst_size_to_pretty1000(bytes, arr_size, &suf);
    return string_printf(format, val, suffixes[suf]);
    }

template <size_t arr_size>
std::string size_to_pretty1024(long long bytes, const char* NTST_RESTRICT format = "%.2f %s", const char* NTST_RESTRICT suffixes[arr_size] = {"B", "KiB", "MiB", "TiB", "PiB"})
    {
    unsigned suf;
    double val = ntst_size_to_pretty1024(bytes, arr_size, &suf);
    return string_printf(format, val, suffixes[suf]);
    }
#endif

template<size_t MAX_SIZE>
class NextOrMinStrFinder
    {
private:
    enum State
        {
        FIND_FIRST,
        FIND_NEXT,
        FIND_MIN
        };
public:
    NextOrMinStrFinder(const char* cur)
        {
        Init(cur);
        }
    void Init(const char* cur)
        {
        cur_str = cur;
        state = FIND_FIRST;
        }
    void PutStr(const char* new_str)
        {
        /*strcmp - Positive value if first is greater than second. */
        switch (state)
            {
            case FIND_FIRST:
                if (strcmp(new_str, cur_str) > 0)
                    state = FIND_NEXT;
                else /* new_str <= cur_str */
                    state = FIND_MIN;
                ntst_strncpy(tmp_str, new_str, MAX_SIZE);
                break;
            case FIND_NEXT:
                if (strcmp(tmp_str, new_str) > 0
                    && strcmp(new_str, cur_str) > 0)
                    {
                    /* tmp_str > new_str > cur_str
                       tmp_str = new_str */
                    ntst_strncpy(tmp_str, new_str, MAX_SIZE);
                    }
                break;
            case FIND_MIN:
                if (strcmp(new_str, cur_str) > 0)
                    {
                    /* new_str > cur_str
                       tmp_str = new_str */
                    ntst_strncpy(tmp_str, new_str, MAX_SIZE);
                    state = FIND_NEXT;
                    }
                else if (strcmp(tmp_str, new_str) > 0)
                    {
                    ntst_strncpy(tmp_str, new_str, MAX_SIZE);
                    }
                break;
            }
        }
    const char* GetStr() const
        {
        if (state != FIND_FIRST)
            return tmp_str;
        return NULL;
        }
private:
    const char* cur_str;
    State state;
    char tmp_str[MAX_SIZE];
    };

} /* namespace ntst */

#ifdef ENTST_ENABLE_QT_USE
#include <QtGlobal>
#include <QString>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QByteArray>
inline std::string QStringToStdUtf8(const QString& str)
    {
    QByteArray tmp = str.toUtf8();
    return std::string(tmp.constData(), tmp.size());
    }
#else
// Since qt5 toStdString() uses utf8
#define QStringToStdUtf8(str) str.toStdString()
#endif // (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
template <size_t arr_size>
QString QStringFromStdUtf8(char const (&str)[arr_size])
    {
    return QString::fromUtf8(str, arr_size - NTST_ONE_NULL_TERM);
    }
inline QString QStringFromStdUtf8(const std::string& str)
    {
    return QString::fromUtf8(str.c_str(), str.size());
    }
#endif // ENTST_ENABLE_QT_USE

#ifdef ENTST_ENABLE_WX_USE
#include <wx/string.h>
inline std::string WxStringToStdUtf8(const wxString& str)
    {
    const wxScopedCharBuffer buf = str.ToUTF8();
    return std::string(buf.data(), buf.length());
    }
#endif

#endif
