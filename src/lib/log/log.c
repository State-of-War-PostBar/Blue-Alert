/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2021 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                                   published by mhtvsSFrpHdE.                                   *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*       You should have received a copy of the license along with the source code of this        *
*  program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.   *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                         https://github.com/State-of-War-PostBar/sowr.                          *
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "log.h"

#include "../container/string.h"
#include "../data/cstring.h"

#ifdef SOWR_BUILD_DEBUG
    static sowr_File log_file;
    static sowr_File log_console;
    static sowr_LogLockFunc lock_func;

    thread_local static sowr_String message_buf;

    static const char *const SOWR_LOGLVL_STRINGS[] =
    {
        "TRACE",
        "DEBUG",
        "INFO ",
        "WARN ",
        "ERROR",
        "FATAL"
    };

    static const char *const SOWR_LOG_COLORCODES[] =
    {
        "\x1b[90m",
        "\x1b[32m",
        "\x1b[94m",
        "\x1b[33m",
        "\x1b[31m",
        "\x1b[35m"
    };

    static const char *const SOWR_LOG_COLORCODE_RESET = "\x1b[0m";
#endif

void
sowr_Logger_Init( sowr_File file, sowr_LogLockFunc lock )
{
#ifdef SOWR_BUILD_DEBUG
    log_file = file;
    log_console = (sowr_File) SOWR_FILE_STDOUT;
    lock_func = lock;
    message_buf = sowr_String_CreateS();
    sowr_String_ExpandUntilOnce(&message_buf, 64ULL);
#endif
}

void
sowr_Logger_ThrdInit( void )
{
#ifdef SOWR_BUILD_DEBUG
    message_buf = sowr_String_CreateS();
    sowr_String_ExpandUntilOnce(&message_buf, 64ULL);
#endif
}

void
sowr_Logger_Log( sowr_LogLevel level, const char *file, int line, const char *message )
{
#ifdef SOWR_BUILD_DEBUG
    time_t raw_time = time(NULL);
    struct tm loc_time = { 0 };
#ifdef SOWR_TARGET_WINDOWS
    localtime_s(&loc_time, &raw_time);
#else
    localtime_r(&raw_time, &loc_time);
#endif
    
    message_buf.length += strftime(message_buf.ptr, message_buf.capacity, "%d-%m-%y %H:%M:%S", &loc_time);
    sowr_String_PushC(&message_buf, ' ');
    size_t colorcode = message_buf.length;
    sowr_String_PushS(&message_buf, SOWR_LOGLVL_STRINGS[level]);
    sowr_String_PushC(&message_buf, ' ');
    sowr_String_PushS(&message_buf, file);
    sowr_String_PushC(&message_buf, ':');
    sowr_CString_IToA(&message_buf, line, 10U);
    sowr_String_PushC(&message_buf, ' ');
    sowr_String_PushS(&message_buf, message);
    sowr_String_PushC(&message_buf, '\n');

    lock_func(true);
    sowr_File_WriteContent(log_file, message_buf.ptr, message_buf.length);
    lock_func(false);
    sowr_String_InsertS(&message_buf, colorcode, SOWR_LOG_COLORCODES[level]);
    sowr_String_InsertS(&message_buf, colorcode + 6ULL + 5ULL, SOWR_LOG_COLORCODE_RESET);
    sowr_File_WriteContent(log_console, message_buf.ptr, message_buf.length);

    sowr_String_Clear(&message_buf);
#endif
}

void
sowr_Logger_LogG( sowr_LogLevel level, const char *file, int line, size_t count, ... )
{
#ifdef SOWR_BUILD_DEBUG
    time_t raw_time = time(NULL);
    struct tm loc_time = { 0 };
#ifdef SOWR_TARGET_WINDOWS
    localtime_s(&loc_time, &raw_time);
#else
    localtime_r(&raw_time, &loc_time);
#endif

    message_buf.length += strftime(message_buf.ptr, message_buf.capacity, "%d-%m-%y %H:%M:%S", &loc_time);
    sowr_String_PushC(&message_buf, ' ');
    size_t colorcode = message_buf.length;
    sowr_String_PushS(&message_buf, SOWR_LOGLVL_STRINGS[level]);
    sowr_String_PushC(&message_buf, ' ');
    sowr_String_PushS(&message_buf, file);
    sowr_String_PushC(&message_buf, ':');
    sowr_CString_IToA(&message_buf, line, 10U);
    sowr_String_PushC(&message_buf, ' ');

    va_list args;
    va_start(args, count);

    sowr_CString_ComposeV(&message_buf, count, &args);
    sowr_String_PushC(&message_buf, '\n');
    lock_func(true);
    sowr_File_WriteContent(log_file, message_buf.ptr, message_buf.length);
    lock_func(false);
    sowr_String_InsertS(&message_buf, colorcode, SOWR_LOG_COLORCODES[level]);
    sowr_String_InsertS(&message_buf, colorcode + 5ULL + 5ULL, SOWR_LOG_COLORCODE_RESET);
    sowr_File_WriteContent(log_console, message_buf.ptr, message_buf.length);

    va_end(args);
    sowr_String_Clear(&message_buf);
#endif
}

void
sowr_Logger_Destroy( void )
{
#ifdef SOWR_BUILD_DEBUG
    sowr_String_DestroyS(&message_buf);
#endif
}

void
sowr_Logger_ThrdDestroy( void )
{
#ifdef SOWR_BUILD_DEBUG
    sowr_String_DestroyS(&message_buf);
#endif
}
