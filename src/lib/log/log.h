/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, some rights reserved.                              *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created, intended to be useful, but without any warranty.          *
* For more information, please forward to the license.                                           *
*                                                                                                *
* You should have received a copy of the license along with the source code of this              *
* program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.    *
*                                                                                                *
* Mission is successfully completed.                                                             *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_LOG_LOG_H
#define SOWR_LIB_LOG_LOG_H

#include <pch.h>

#include <logger/log.h>

#ifdef SOWR_BUILD_DEBUG
    #define SOWR_LOG_TRACE(...)  log_trace(__VA_ARGS__)
    #define SOWR_LOG_DEBUG(...)  log_debug(__VA_ARGS__)
    #define SOWR_LOG_INFO(...)   log_info(__VA_ARGS__)
    #define SOWR_LOG_WARN(...)   log_warn(__VA_ARGS__)
    #define SOWR_LOG_ERROR(...)  log_error(__VA_ARGS__)
    #define SOWR_LOG_FATAL(...)  log_fatal(__VA_ARGS__)

    #define SOWR_LOG_LEVEL_TRACE LOG_TRACE
    #define SOWR_LOG_LEVEL_INFO  LOG_INFO
    #define SOWR_LOG_LEVEL_DEBUG LOG_DEBUG
    #define SOWR_LOG_LEVEL_WARN  LOG_WARN
    #define SOWR_LOG_LEVEL_ERROR LOG_ERROR
    #define SOWR_LOG_LEVEL_FATAL LOG_FATAL
#else
    #define SOWR_LOG_TRACE(...)
    #define SOWR_LOG_INFO(...)
    #define SOWR_LOG_WARN(...)
    #define SOWR_LOG_ERROR(...)
    #define SOWR_LOG_FATAL(...)
#endif

#define SOWR_DEFAULT_LOG_BUFFER_SIZE 512

#endif // !SOWR_LIB_LOG_LOG_H
