/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2023 State of War Baidu Postbar, some rights reserved.                *
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

#ifndef BLRT_LOG_PROFILE_H
#define BLRT_LOG_PROFILE_H

#include <pch.h>

///
/// \brief Initialize the profiler
///
/// Initialize the profiler for the whole program. This function needs to be called for once and only once for the profiler to work.<BR />
/// <B>On Posix systems, this function does nothing.</B>
///
/// \note You do not have to call it for different threads, only one thread needs to do so.
///
void
blrt_InitProfiler( void );

///
/// \brief Profile a function
///
/// Start or stop profiling a function. When called for the first time, profiling starts.
/// When called for the second time (in the same thread), the profiling stops.<BR />
/// The profile result is outputed as <I>[Logger debug prefix] Profiling "function name" ("start line" to "stop line" in "file name") took "time" ms.</I><BR />
/// This function is thread-safe, every thread has their own profile record.
///
/// \note It is recommanded to use the macros \a BLRT_START_PROFILE() and \a BLRT_STOP_PROFILE() instead.
///
/// \param caller_file File of the caller in.
/// \param caller_func Function to profile.
/// \param line The line when profiling starts or stops.
///
void
blrt_ProfileFunc( const char *caller_file, const char *caller_func, int line );

#ifdef BLRT_BUILD_DEBUG
    #define BLRT_START_PROFILE() blrt_ProfileFunc(__FILE__, __func__, __LINE__)
    #define BLRT_STOP_PROFILE()  blrt_ProfileFunc(__FILE__, __func__, __LINE__)
#else
    #define BLRT_START_PROFILE()
    #define BLRT_STOP_PROFILE()
#endif

#endif // !BLRT_LOG_PROFILE_H
