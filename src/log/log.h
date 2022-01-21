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

#ifndef BLRT_LIB_LOG_LOG_H
#define BLRT_LIB_LOG_LOG_H

#include <pch.h>

#include "../lib/io/filesystem/fio.h"
#include "../lib/thread/lock.h"

typedef enum blrt_LogLevel
{
    BLRT_LOGLVL_TRACE,
    BLRT_LOGLVL_DEBUG,
    BLRT_LOGLVL_INFO,
    BLRT_LOGLVL_WARN,
    BLRT_LOGLVL_ERROR,
    BLRT_LOGLVL_FATAL
} blrt_LogLevel;

typedef void (*blrt_LogLockFunc)( bool );

///
/// \brief Initialize logger
///
/// Initialize the logger.
///
void
blrt_Logger_Init( void );

///
/// \brief Initialize logger
///
/// Initialize the logger thread-locally.
///
void
blrt_Logger_ThrdInit( void );

///
/// \brief Log an event
///
/// Log an event.
///
/// \param level Level of the event
/// \param file Source file of the event
/// \param line Line of the source file of the event
/// \param message Message to log
///
void
blrt_Logger_Log( blrt_LogLevel level, const char *file, int line, const char *message );

///
/// \brief Log an event
///
/// \param level Level of the event
/// \param file Source file of the event
/// \param line Line of the source file of the event
/// \param count Number of parameters
///
void
blrt_Logger_LogG( blrt_LogLevel level, const char *file, int line, size_t count, ... );

///
/// \brief Destroy the logger
///
/// Destroy the logger.
///
void
blrt_Logger_Destroy( void );

///
/// \brief Destroy the logger
///
/// Destroy the logger thread-locally.
///
void
blrt_Logger_ThrdDestroy( void );

#ifdef BLRT_BUILD_DEBUG
    #define BLRT_LOG_TRACE(str) blrt_Logger_Log(BLRT_LOGLVL_TRACE, __FILE__, __LINE__, (str))
    #define BLRT_LOG_DEBUG(str) blrt_Logger_Log(BLRT_LOGLVL_DEBUG, __FILE__, __LINE__, (str))
    #define BLRT_LOG_INFO(str)  blrt_Logger_Log(BLRT_LOGLVL_INFO, __FILE__, __LINE__, (str))
    #define BLRT_LOG_WARN(str)  blrt_Logger_Log(BLRT_LOGLVL_WARN, __FILE__, __LINE__, (str))
    #define BLRT_LOG_ERROR(str) blrt_Logger_Log(BLRT_LOGLVL_ERROR, __FILE__, __LINE__, (str))
    #define BLRT_LOG_FATAL(str) blrt_Logger_Log(BLRT_LOGLVL_FATAL, __FILE__, __LINE__, (str))

    #define BLRT_LOG_TRACE_G(count, ...)  blrt_Logger_LogG(BLRT_LOGLVL_TRACE, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define BLRT_LOG_DEBUG_G(count, ...)  blrt_Logger_LogG(BLRT_LOGLVL_DEBUG, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define BLRT_LOG_INFO_G(count, ...)   blrt_Logger_LogG(BLRT_LOGLVL_INFO, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define BLRT_LOG_WARN_G(count, ...)   blrt_Logger_LogG(BLRT_LOGLVL_WARN, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define BLRT_LOG_ERROR_G(count, ...)  blrt_Logger_LogG(BLRT_LOGLVL_ERROR, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define BLRT_LOG_FATAL_G(count, ...)  blrt_Logger_LogG(BLRT_LOGLVL_FATAL, __FILE__, __LINE__, (count), __VA_ARGS__)

    #define BLRT_LOG_ARG BLRT_MAKE_GENERIC
#else
    #define BLRT_LOG_TRACE(str)
    #define BLRT_LOG_DEBUG(str)
    #define BLRT_LOG_INFO(str)
    #define BLRT_LOG_WARN(str)
    #define BLRT_LOG_ERROR(str)
    #define BLRT_LOG_FATAL(str)

    #define BLRT_LOG_TRACE_G(count, ...)
    #define BLRT_LOG_DEBUG_G(count, ...)
    #define BLRT_LOG_INFO_G(count, ...)
    #define BLRT_LOG_WARN_G(count, ...)
    #define BLRT_LOG_ERROR_G(count, ...)
    #define BLRT_LOG_FATAL_G(count, ...)

    #define BLRT_LOG_ARG
#endif

#endif // !BLRT_LIB_LOG_LOG_H
