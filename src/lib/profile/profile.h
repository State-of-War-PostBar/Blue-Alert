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

#ifndef SOWR_LIB_PROFILE_PROFILE_H
#define SOWR_LIB_PROFILE_PROFILE_H

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
sowr_InitProfiler( void );

///
/// \brief Profile a function
///
/// Start or stop profiling a function. When called for the first time, profiling starts.
/// When called for the second time (in the same thread), the profiling stops.<BR />
/// The profile result is outputed as <I>[Logger debug prefix] Profiling "function name" ("start line" to "stop line" in "file name") took "time" ms.</I><BR />
/// This function is thread-safe, every thread has their own profile record.
///
/// \note It is recommanded to use the macros \a SOWR_START_PROFILE() and \a SOWR_STOP_PROFILE() instead.
///
/// \param caller_file File of the caller in.
/// \param caller_func Function to profile.
/// \param line The line when profiling starts or stops.
///
void
sowr_ProfileFunc( const char *caller_file, const char *caller_func, int line );

#ifdef SOWR_BUILD_DEBUG
    #define SOWR_START_PROFILE() sowr_ProfileFunc(__FILE__, __func__, __LINE__)
    #define SOWR_STOP_PROFILE()  sowr_ProfileFunc(__FILE__, __func__, __LINE__)
#else
    #define SOWR_START_PROFILE()
    #define SOWR_STOP_PROFILE()
#endif

#endif // !SOWR_LIB_PROFILE_PROFILE_H
