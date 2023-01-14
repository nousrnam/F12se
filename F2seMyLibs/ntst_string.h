#ifndef GNTST_STRING_H
#define GNTST_STRING_H

#include "ntst_base.h"

#include <stddef.h> /* size_t C/C++ */
#include <string.h>
#include <stdarg.h>

#define NTST_ONE_NULL_TERM 1

// Необходимость создания этой обертки над isspace взрывает мозг.
// isspace принимает int, но передаваться должно значение из диапазона unsigned char.
// isspace может быть реализован как макрос над массивом булок. Если будет передан
// char < 0, то это знатный бабах - чтение памяти до массива.
// Ввёл макрос после кучи предупреждений array subscript has type 'char' [-Wchar-subscripts]
#define NTST_ISSPACE(ch) isspace((unsigned char)(ch))

// Просто макрос для себя, чтобы проверять символ от '0' до '9', без локалей и т.п.
// C99: the value of each character after 0 in the above list of decimal digits shall be one greater than the value of the previous.
#define NTST_ISDIGIT(ch) (((ch) <= '9') && ((ch) >= '0'))

#ifdef __cplusplus
extern "C" {
#else
int ntst_strhasprefix(const char* NTST_RESTRICT string, const char* NTST_RESTRICT prefix) NTST_NONNULL_ARGS;
#endif

int ntst_ascii_tolower(int ch);

/* '0'->0, 'F'->0xF, negative on failure */
int ntst_hex_char_to_int(char c);
/* '00'->0, 'FF'->0xFF, negative on failure(-1 fail high, -2 fail low) */
int ntst_hex_char_pair_to_int(char c_high, char c_low);
void ntst_byte_to2char(unsigned char byte, char* out) NTST_NONNULL_ARGS;

/**
 * Под виндой переполнение это ошибка -1. По стандарту возвращает сколько было бы записано.
 */
int ntst_snprintf(char* NTST_RESTRICT buff, unsigned max_len, const char* NTST_RESTRICT format, ...)
NTST_ATTRIB_PRINTF_FMT(3,4)
;
int ntst_vsnprintf(char* NTST_RESTRICT buff, unsigned max_len, const char* NTST_RESTRICT format, va_list vlist);

/* For ntst_print_... only. printf functions may add locale specific separators.
 +1 division round up, +1 for '\0', +1 for '-'
*/
#define NTST_PRINT_UNSIGNED_SIZE(v) (sizeof(v)*CHAR_BIT*28/93+2)
#define NTST_PRINT_SIGNED_SIZE(v) (sizeof(v)*CHAR_BIT*28/93+3)
/* Return numbers of char written, including '\0' for ...z */
/* TODO: Правила очень мутные получились, особенно pad...z. Надо из собрать, описать и написать тесты. */
unsigned ntst_print_uint_dec_unsafe(char* buff, unsigned val) NTST_NONNULL_ARGS;
unsigned ntst_print_uint_decz_unsafe(char* buff, unsigned val) NTST_NONNULL_ARGS;
unsigned ntst_print_int_dec_unsafe(char* buff, int val) NTST_NONNULL_ARGS;
unsigned ntst_print_int_decz_unsafe(char* buff, int val) NTST_NONNULL_ARGS;
unsigned ntst_print_uint_dec(char* buff, unsigned max_len, unsigned val) NTST_NONNULL_ARGS;
unsigned ntst_print_uint_decz(char* buff, unsigned max_len, unsigned val) NTST_NONNULL_ARGS;
unsigned ntst_print_int_dec(char* buff, unsigned max_len, int val) NTST_NONNULL_ARGS;
unsigned ntst_print_int_decz(char* buff, unsigned max_len, int val) NTST_NONNULL_ARGS;
unsigned ntst_print_pad_uint_dec_unsafe(char* buff, unsigned min_len, unsigned val, char pad) NTST_NONNULL_ARGS;
unsigned ntst_print_pad_uint_decz_unsafe(char* buff, unsigned min_len, unsigned val, char pad) NTST_NONNULL_ARGS;
unsigned ntst_print_pad_int_dec_unsafe(char* buff, unsigned min_len, int val, char pad) NTST_NONNULL_ARGS;
unsigned ntst_print_pad_int_decz_unsafe(char* buff, unsigned min_len, int val, char pad) NTST_NONNULL_ARGS;
unsigned ntst_print_pad_uint_dec(char* buff, unsigned min_len, unsigned max_len, unsigned val, char pad) NTST_NONNULL_ARGS;
unsigned ntst_print_pad_uint_decz(char* buff, unsigned min_len, unsigned max_len, unsigned val, char pad) NTST_NONNULL_ARGS;
unsigned ntst_print_pad_int_dec(char* buff, unsigned min_len, unsigned max_len, int val, char pad) NTST_NONNULL_ARGS;
unsigned ntst_print_pad_int_decz(char* buff, unsigned min_len, unsigned max_len, int val, char pad) NTST_NONNULL_ARGS;

/**
  * Соединяет '\0' терминированные строки, пока не встретит NULL или не кончится max_len
  * Возвращает размер буфера нужный чтобы вместить все строки без терминатора.
  * Результат без терминатора!
  * buff == NULL можно если max_len == 0
  */
size_t ntst_str_concat(char* NTST_RESTRICT buff, size_t max_len, ...);

/**
 * @brief ntst_strncpy - Копирует не более (count - 1) символов из src в dest и добавляет 0 терминатор, если count > 0.
 * Почему не strncpy:
 * If count is reached before the entire string src was copied, the resulting character array is not null-terminated.
 * Ненавижу тех кто иногда не пришмякивает нули.
 * If, after copying the terminating null character from src, count is not reached, additional null characters are written to dest until the total of count characters have been written.
 * Почти 4 килобайта нулей в пути к файлу в линуксе. Да за такое на помоечку сразу.
 * @param dest
 * @param src
 * @param count
 * @return Число скопированных символов без учета 0 терминатора(==strlen). Или 0 если count == 0
 */
size_t ntst_strncpy(char* NTST_RESTRICT dest, const char* NTST_RESTRICT src, size_t count);

size_t ntst_str10toi(const char* NTST_RESTRICT str, int* NTST_RESTRICT result) NTST_NONNULL_ARGS;
size_t ntst_str10tol(const char* NTST_RESTRICT str, long* NTST_RESTRICT result) NTST_NONNULL_ARGS;
size_t ntst_str10toll(const char* NTST_RESTRICT str, long long* NTST_RESTRICT result) NTST_NONNULL_ARGS;
size_t ntst_str10tou(const char* NTST_RESTRICT str, unsigned* NTST_RESTRICT result) NTST_NONNULL_ARGS;
size_t ntst_strn10toi(const char* NTST_RESTRICT str, size_t len, int* NTST_RESTRICT result) NTST_NONNULL_ARGS;
size_t ntst_strn10tol(const char* NTST_RESTRICT str, size_t len, long* NTST_RESTRICT result) NTST_NONNULL_ARGS;
size_t ntst_strn10tou(const char* NTST_RESTRICT str, size_t len, unsigned* NTST_RESTRICT result) NTST_NONNULL_ARGS;
size_t ntst_strtof(const char* NTST_RESTRICT str, float* NTST_RESTRICT result) NTST_NONNULL_ARGS;
size_t ntst_strtod(const char* NTST_RESTRICT str, double* NTST_RESTRICT result) NTST_NONNULL_ARGS;

/**
  * Результат без нультерминатора. Поэтому str нужно минимум blob_size*2. +1 если ручками добавляем '\0'
  */
void ntst_blobtohexstr(const void* NTST_RESTRICT blob, size_t blob_size, char* NTST_RESTRICT str);
/**
  * '\'  - \\
  * '\0' - \00
  * '\a' - \a
  * '\b' - \b
  * '\t' - \t
  * '\n' - \n
  * '\v' - \v
  * '\f' - \f
  * '\r' - \r
  *  max result size blob_size*NTST_ESCAPED_CHAR_MAX_SIZE
  */
#define NTST_ESCAPED_CHAR_MAX_SIZE 3
size_t ntst_escape_char(char c, void* out) NTST_NONNULL_ARGS;
size_t ntst_escape_char_ascii(char c, void* out) NTST_NONNULL_ARGS;
size_t ntst_blobtoescapedstr(const void* NTST_RESTRICT blob, size_t blob_size, char* NTST_RESTRICT str);
/* String allocated with NTST_MALLOC, must be freed with NTST_FREE */
char* ntst_blob_to_str(const char* NTST_RESTRICT blob_name, const void* NTST_RESTRICT blob, size_t* NTST_RESTRICT size);

/**
 * @brief ntst_strchr_after_n пропускает num символов равных c и возвращает указатель на строку после последнего.
 * @return NULL если в строке нет num симолов c.
 */
char* ntst_strchr_after_n(const char* str, int c, unsigned num) NTST_NONNULL_ARGS;
char* ntst_strnchr(const char* str, int ch, size_t n) NTST_NONNULL_ARGS;
size_t ntst_strchr_cnt(const char* str, char c) NTST_NONNULL_ARGS;
size_t ntst_strnlen(const char* str, size_t n) NTST_NONNULL_ARGS;
/**
 * @brief Ищет символ в блобе.
 * @param data указатель на блоб.
 * @param len размер блоба.
 * @param c искомый символ.
 * @return Указатель на найденный символ или NULL, если не нашли.
 */
void* ntst_memchr(const void* NTST_RESTRICT data, size_t len, char c) NTST_NONNULL_ARGS;
/**
 * @brief Ищет символ в блобе, пропуская n первых.
 * @param data указатель на блоб.
 * @param len размер блоба.
 * @param c искомый символ.
 * @param n сколько символов пропускаем
 * @return Указатель на найденный символ или NULL, если не нашли.
 */
void* ntst_memchr_skipn(const void* NTST_RESTRICT data, size_t len, char c, unsigned n) NTST_NONNULL_ARGS;
void* ntst_memchr_it_skipn(const void* NTST_RESTRICT dataBegin, const void* NTST_RESTRICT dataEnd, char c, unsigned n) NTST_NONNULL_ARGS;
/**
 * @brief Ищет подблоб в блобе.
 * @param data указатель на блоб.
 * @param len размер блоба.
 * @param subdata указатель на подблоб.
 * @param sublen размер подблоба.
 * @return Указатель на найденный подблоб или NULL, если не нашли.
 */
void* ntst_memmem(const void* NTST_RESTRICT data, size_t len, const void* NTST_RESTRICT subdata, size_t sublen) NTST_NONNULL_ARGS;

/**
 * @brief Результаты разбора строки с числом с фиксированной точкой.
 * Бывает такое, что всегда 1-2 числа до точки и 1-2 после. Легко может поместиться в uint16_t,
 * но приходится использовать float, а он при обратном преобразовании в строку выдает 9 в периоде.
 * TODO если/когда понадобится обработка конкретных ошибок добавить int в структуру вместо выставления errno.
 */
struct FixedPointInfo
{
    size_t totalLen; /**< Сколько всего символов прочитали. 0 если не смогли распарсить. */
    unsigned char partLen; /**< Сколько символов было в дробной части вместе с разделителем. Чтобы проверять что ожидаемое число. 0 если не смогли распарсить. */
    char separator; /**< Какой символ разделял части. Обычно . или , */
};

/**
 * @brief Пытается распарсить число с фиксированной точкой в десятичной системе.
 * Для простоты всё, что не NTST_ISDIGIT, является разделителем.
 * Следующий не NTST_ISDIGIT или достижение len завершает парсинг.
 * '\0' на всякий случай тоже завершит парсинг. Не представляю его в качестве разделителя.
 * @param data строка.
 * @param len длина строки.
 * @param out указатель для записи результатов.
 * @return 0 в случае ошибок. Успех или нет смотреть по out->totalLen
 */
unsigned ntst_mem_fixedp_to_uint(const void* NTST_RESTRICT dataBegin, size_t len, struct FixedPointInfo* NTST_RESTRICT out) NTST_NONNULL_ARGS;
unsigned ntst_mem_it_fixedp_to_uint(const void* NTST_RESTRICT dataBegin, const void* NTST_RESTRICT dataEnd, struct FixedPointInfo* NTST_RESTRICT out) NTST_NONNULL_ARGS;

double ntst_size_to_pretty1000(long long bytes, unsigned suffixes_len, unsigned* suffix_index);
double ntst_size_to_pretty1024(long long bytes, unsigned suffixes_len, unsigned* suffix_index);
int ntst_pretty_print_size1000(long long bytes, char* NTST_RESTRICT buff, unsigned buff_len, const char* NTST_RESTRICT format, const char** NTST_RESTRICT suffixes, unsigned suffixes_len);
int ntst_pretty_print_size1024(long long bytes, char* NTST_RESTRICT buff, unsigned buff_len, const char* NTST_RESTRICT format, const char** NTST_RESTRICT suffixes, unsigned suffixes_len);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif
