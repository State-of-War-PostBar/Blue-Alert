/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*           Blue Alert is a free software. You can freely do whatever you want with it           *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*            Blue Alert is created, intended to be useful, but without any warranty.             *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*           <https://github.com/State-of-War-PostBar/Blue-Alert/blob/master/LICENSE>.            *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                     <https://github.com/State-of-War-PostBar/Blue-Alert>.                      *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "log.h"

#include "../lib/container/string.h"
#include "../lib/text/compose.h"

#ifdef BLRT_BUILD_DEBUG
    static const char *const BLRT_LOGLVL_STRINGS[] =
    {
        "TRACE",
        "DEBUG",
        "INFO ",
        "WARN ",
        "ERROR",
        "FATAL"
    };

    static const char *const BLRT_LOG_COLORCODES[] =
    {
        "\x1b[90m",
        "\x1b[32m",
        "\x1b[94m",
        "\x1b[33m",
        "\x1b[31m",
        "\x1b[35m"
    };

    static const char *const BLRT_LOG_COLORCODE_RESET = "\x1b[0m";

    static const char *const BLRT_LOG_FILE_NAME = "./blrt.log";
    static bool log_available;
    static blrt_File log_file;
    static blrt_File log_console;
    static blrt_CriticalSection log_file_mtx;

    static
    void
    blrt_LockLogFile( bool lock )
    {
        lock ?
            blrt_EnterCriticalSection(&log_file_mtx)
        :
            blrt_LeaveCriticalSection(&log_file_mtx);
    };
#endif

thread_local static blrt_String log_message_buffer;

void
blrt_Logger_Init( void )
{
#ifdef BLRT_BUILD_DEBUG
    log_message_buffer = blrt_String_CreateS();
    blrt_String_ExpandUntilOnce(&log_message_buffer, 64ULL);

    log_console = (blrt_File) BLRT_FILE_STDOUT;
    log_file = blrt_File_OpenOrCreate(BLRT_LOG_FILE_NAME, BLRT_FIO_WRITE_TRUNCATE);
    if (!log_file)
        perror("Failed to create a log file, logging will not be available");
    else
    {
        log_available = true;
        blrt_InitCriticalSection(&log_file_mtx);
    }
#endif
}

void
blrt_Logger_ThrdInit( void )
{
#ifdef BLRT_BUILD_DEBUG
    log_message_buffer = blrt_String_CreateS();
    blrt_String_ExpandUntilOnce(&log_message_buffer, 64ULL);
#endif
}

void
blrt_Logger_Log( blrt_LogLevel level, const char *file, int line, const char *message )
{
#ifdef BLRT_BUILD_DEBUG
    time_t raw_time = time(NULL);
    struct tm loc_time = { 0 };
#ifdef BLRT_TARGET_WINDOWS
    localtime_s(&loc_time, &raw_time);
#else
    localtime_r(&raw_time, &loc_time);
#endif

    log_message_buffer.length += strftime(log_message_buffer.ptr, log_message_buffer.capacity, "%d-%m-%y %H:%M:%S", &loc_time);
    blrt_String_PushC(&log_message_buffer, ' ');
    size_t colorcode = log_message_buffer.length;
    blrt_String_PushS(&log_message_buffer, BLRT_LOGLVL_STRINGS[level]);
    blrt_String_PushC(&log_message_buffer, ' ');
    blrt_String_PushS(&log_message_buffer, file);
    blrt_String_PushC(&log_message_buffer, ':');
    blrt_StringCompose_IToA(&log_message_buffer, line, 10U);
    blrt_String_PushC(&log_message_buffer, ' ');
    blrt_String_PushS(&log_message_buffer, message);
    blrt_String_PushC(&log_message_buffer, '\n');

    if (log_available)
        blrt_LockLogFile(true);
    blrt_File_WriteContent(log_file, log_message_buffer.ptr, log_message_buffer.length);
    if (log_available)
        blrt_LockLogFile(false);
    blrt_String_InsertS(&log_message_buffer, colorcode, BLRT_LOG_COLORCODES[level]);
    blrt_String_InsertS(&log_message_buffer, colorcode + 6ULL + 5ULL, BLRT_LOG_COLORCODE_RESET);
    blrt_File_WriteContent(log_console, log_message_buffer.ptr, log_message_buffer.length);

    blrt_String_Clear(&log_message_buffer);
#endif
}

void
blrt_Logger_LogG( blrt_LogLevel level, const char *file, int line, size_t count, ... )
{
#ifdef BLRT_BUILD_DEBUG
    time_t raw_time = time(NULL);
    struct tm loc_time = { 0 };
#ifdef BLRT_TARGET_WINDOWS
    localtime_s(&loc_time, &raw_time);
#else
    localtime_r(&raw_time, &loc_time);
#endif

    log_message_buffer.length += strftime(log_message_buffer.ptr, log_message_buffer.capacity, "%d-%m-%y %H:%M:%S", &loc_time);
    blrt_String_PushC(&log_message_buffer, ' ');
    size_t colorcode = log_message_buffer.length;
    blrt_String_PushS(&log_message_buffer, BLRT_LOGLVL_STRINGS[level]);
    blrt_String_PushC(&log_message_buffer, ' ');
    blrt_String_PushS(&log_message_buffer, file);
    blrt_String_PushC(&log_message_buffer, ':');
    blrt_StringCompose_IToA(&log_message_buffer, line, 10U);
    blrt_String_PushC(&log_message_buffer, ' ');

    va_list args;
    va_start(args, count);

    blrt_StringCompose_ComposeV(&log_message_buffer, count, &args);
    blrt_String_PushC(&log_message_buffer, '\n');
    if (log_available)
        blrt_LockLogFile(true);
    blrt_File_WriteContent(log_file, log_message_buffer.ptr, log_message_buffer.length);
    if (log_available)
        blrt_LockLogFile(false);
    blrt_String_InsertS(&log_message_buffer, colorcode, BLRT_LOG_COLORCODES[level]);
    blrt_String_InsertS(&log_message_buffer, colorcode + 5ULL + 5ULL, BLRT_LOG_COLORCODE_RESET);
    blrt_File_WriteContent(log_console, log_message_buffer.ptr, log_message_buffer.length);

    va_end(args);
    blrt_String_Clear(&log_message_buffer);
#endif
}

void
blrt_Logger_Destroy( void )
{
#ifdef BLRT_BUILD_DEBUG
    blrt_String_DestroyS(&log_message_buffer);
    if (log_available)
    {
        blrt_DestroyCriticalSection(&log_file_mtx);
        log_file = BLRT_INVALID_FILE_DESCRIPTOR;
        log_available = false;
    }
#endif
}

void
blrt_Logger_ThrdDestroy( void )
{
#ifdef BLRT_BUILD_DEBUG
    blrt_String_DestroyS(&log_message_buffer);
#endif
}
