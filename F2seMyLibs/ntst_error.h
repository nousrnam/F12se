#ifndef GNTST_ERROR_H
#define GNTST_ERROR_H

#include <errno.h>

/*
http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
https://msdn.microsoft.com/en-us/library/windows/desktop/ms681382%28v=vs.85%29.aspx

Напоминание поскольку стал забывать что и зачем сделал:
Младшие 16 бит выделены под мои коды ошибок, которые по возможности совпадают с errno.
Неизвестные или неиспользуемые errno переносятся в старшие 16 бит.
Ошибки WinApi хранятся как отрицательные числа. Вообще стоило наверно за основу брать errno,
а уже для моих ошибок использовать старшие биты. Но системные коды ошибок надо
объединять с ошибками STM, поэтому по хорошему надо как-то всё переделать единообразно.
Ещё поскольку не весь код делаю полностью переносимым возможно стоит ввести что-то
типа ntst_sys_error_t который DWORD под виндой и int в posix.(GetLastError() и errno)
Его и возвращать из функций, а то иногда нужно значение errno, в возвращаю ntst_error_t.
*/

typedef int ntst_error_t;
#define NTST_SYS_ER_OK 0
#if defined(_WIN32)
#include <windows.h>
typedef DWORD ntst_sys_err_t; /**< NTST_SYS_ER_OK если успех, иначе результат GetLastError() */
#ifndef EINPROGRESS
#define EINPROGRESS 112
#endif
#ifndef ENOTCONN
#define ENOTCONN 126
#endif
#ifndef EOVERFLOW
#define EOVERFLOW 132
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK 140
#endif
#else
typedef int ntst_sys_err_t; /**< NTST_SYS_ER_OK если успех, иначе errno */
#endif

#define NTST_ERROR_OK 0
#define NTST_ERROR_EINTR EINTR
#define NTST_ERROR_EINVAL EINVAL /* Invalid argument. */
#define NTST_ERROR_EOVERFLOW EOVERFLOW
#define NTST_ERROR_EAGAIN EAGAIN
#define NTST_ERROR_ENOMEM ENOMEM
#define NTST_ERROR_EBADF EBADF
#define NTST_ERROR_ENOENT ENOENT
#define NTST_ERROR_EINPROGRESS EINPROGRESS
#define NTST_ERROR_EWOULDBLOCK EWOULDBLOCK /* Can be equal to EAGAIN */
#define NTST_ERROR_ENOTCONN ENOTCONN

#if (NTST_ERROR_EAGAIN == NTST_ERROR_EWOULDBLOCK)
#define NTST_NEED_TRY_AGAIN_ERROR(error_code) (error_code == NTST_ERROR_EAGAIN)
#else
#define NTST_NEED_TRY_AGAIN_ERROR(error_code) ((error_code == NTST_ERROR_EAGAIN) || (error_code == NTST_ERROR_EWOULDBLOCK))
#endif

#ifdef __cplusplus
extern "C" {
#endif

ntst_error_t ntst_error_from_errno(int e);

#if defined(_WIN32)

ntst_error_t ntst_error_from_system(DWORD e);
/*DWORD ntst_error_to_system(int e);*/
const char* ntst_error_win_to_string(DWORD e);
#else

ntst_error_t ntst_error_from_system(int e);
/*int ntst_error_to_system(ntst_error_t e);*/
#endif /* #if defined(_WIN32) */

const char* ntst_error_to_string(int e);

#ifdef __cplusplus
}
#endif

#endif
