/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, all rights reserved.                               *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created intended to be useful, but without any warranty.           *
* For more information, please forward to the license.                                           *
*                                                                                                *
* You should have received a copy of the license along with the source code of this              *
* program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.    *
*                                                                                                *
* Mission is successfully completed.                                                             *
*                                                                                                *
**************************************************************************************************/

#include "profile.h"

#include "../log/log.h"

#ifdef SOWR_BUILD_DEBUG
    #ifdef SOWR_TARGET_WINDOWS
        static LARGE_INTEGER sowr_win_profile_timer_frequency;
    #else
        #include <sys/time.h>
        typedef struct timeval sowr_PosixTimeVal;
    #endif
#endif

void
sowr_InitProfiler(void)
{
#if defined SOWR_BUILD_DEBUG && defined SOWR_TARGET_WINDOWS
    QueryPerformanceFrequency(&sowr_win_profile_timer_frequency);
#endif
}

void
sowr_ProfileFunc(const char *caller_file, const char *caller_name, int called_line)
{
#ifdef SOWR_BUILD_DEBUG
    thread_local static double elapsed;
    thread_local static bool first_called = true;
    thread_local static int start_line;

    #ifdef SOWR_TARGET_WINDOWS
        thread_local static LARGE_INTEGER start, stop;

        if (first_called)
        {
            start_line = called_line;
            QueryPerformanceCounter(&start);
        }
    else
        {
            QueryPerformanceCounter(&stop);
            elapsed = (stop.QuadPart - start.QuadPart) * 1000.0f / sowr_win_profile_timer_frequency.QuadPart;
            SOWR_LOG_DEBUG("Profiling %s (Line %d - %d in %s) took %lf ms.", caller_name, start_line + 1, called_line - 1, caller_file, elapsed);
        }
    #else
        thread_local static sowr_PosixTimeVal start, stop;

        if (first_called)
        {
            start_line = called_line;
            gettimeofday(&start, NULL);
        }
    else
        {
            gettimeofday(&stop, NULL);
            elapsed = (stop.tv_sec - start.tv_sec) * 1000.0f + (stop.tv_usec - start.tv_usec) / 1000.0f;
            SOWR_LOG_DEBUG("Profiling %s (Line %d - %d in %s) took %lf ms.", caller_name, start_line + 1, called_line - 1, caller_file, elapsed);
        }
    #endif

    first_called = !first_called;
#endif
}
