/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
*                  any later version of the document released by mhtvsSFrpHdE.                   *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*              <https://github.com/State-of-War-PostBar/sowr/blob/master/LICENSE>.               *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                        <https://github.com/State-of-War-PostBar/sowr>.                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_LOG_LOG_H
#define SOWR_LIB_LOG_LOG_H

#include <pch.h>

#include "../io/filesystem/fio.h"

typedef enum sowr_LogLevel
{
    SOWR_LOGLVL_TRACE,
    SOWR_LOGLVL_DEBUG,
    SOWR_LOGLVL_INFO,
    SOWR_LOGLVL_WARN,
    SOWR_LOGLVL_ERROR,
    SOWR_LOGLVL_FATAL
} sowr_LogLevel;

typedef void (*sowr_LogLockFunc)( bool );

///
/// \brief Initialize logger
///
/// Initialize the logger.
///
/// \param file File for log output
/// \param lock Locking function for writing to the file
///
void
sowr_Logger_Init( sowr_File file, sowr_LogLockFunc lock );

///
/// \brief Initialize logger
///
/// Initialize the logger thread-locally.
///
void
sowr_Logger_ThrdInit( void );

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
sowr_Logger_Log( sowr_LogLevel level, const char *file, int line, const char *message );

///
/// \brief Log an event
///
/// \param level Level of the event
/// \param file Source file of the event
/// \param line Line of the source file of the event
/// \param count Number of parameters
///
void
sowr_Logger_LogG( sowr_LogLevel level, const char *file, int line, size_t count, ... );

///
/// \brief Destroy the logger
///
/// Destroy the logger.
///
void
sowr_Logger_Destroy( void );

///
/// \brief Destroy the logger
///
/// Destroy the logger thread-locally.
///
void
sowr_Logger_ThrdDestroy( void );

#ifdef SOWR_BUILD_DEBUG
    #define SOWR_LOG_TRACE(str) sowr_Logger_Log(SOWR_LOGLVL_TRACE, __FILE__, __LINE__, (str))
    #define SOWR_LOG_DEBUG(str) sowr_Logger_Log(SOWR_LOGLVL_DEBUG, __FILE__, __LINE__, (str))
    #define SOWR_LOG_INFO(str)  sowr_Logger_Log(SOWR_LOGLVL_INFO, __FILE__, __LINE__, (str))
    #define SOWR_LOG_WARN(str)  sowr_Logger_Log(SOWR_LOGLVL_WARN, __FILE__, __LINE__, (str))
    #define SOWR_LOG_ERROR(str) sowr_Logger_Log(SOWR_LOGLVL_ERROR, __FILE__, __LINE__, (str))
    #define SOWR_LOG_FATAL(str) sowr_Logger_Log(SOWR_LOGLVL_FATAL, __FILE__, __LINE__, (str))

    #define SOWR_LOG_TRACE_G(count, ...)  sowr_Logger_LogG(SOWR_LOGLVL_TRACE, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define SOWR_LOG_DEBUG_G(count, ...)  sowr_Logger_LogG(SOWR_LOGLVL_DEBUG, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define SOWR_LOG_INFO_G(count, ...)   sowr_Logger_LogG(SOWR_LOGLVL_INFO, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define SOWR_LOG_WARN_G(count, ...)   sowr_Logger_LogG(SOWR_LOGLVL_WARN, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define SOWR_LOG_ERROR_G(count, ...)  sowr_Logger_LogG(SOWR_LOGLVL_ERROR, __FILE__, __LINE__, (count), __VA_ARGS__)
    #define SOWR_LOG_FATAL_G(count, ...)  sowr_Logger_LogG(SOWR_LOGLVL_FATAL, __FILE__, __LINE__, (count), __VA_ARGS__)
#else
    #define SOWR_LOG_TRACE(str)
    #define SOWR_LOG_DEBUG(str)
    #define SOWR_LOG_INFO(str)
    #define SOWR_LOG_WARN(str)
    #define SOWR_LOG_ERROR(str)
    #define SOWR_LOG_FATAL(str)

    #define SOWR_LOG_TRACE_G(count, ...)
    #define SOWR_LOG_DEBUG_G(count, ...)
    #define SOWR_LOG_INFO_G(count, ...)
    #define SOWR_LOG_WARN_G(count, ...)
    #define SOWR_LOG_ERROR_G(count, ...)
    #define SOWR_LOG_FATAL_G(count, ...)
#endif

#endif // !SOWR_LIB_LOG_LOG_H
