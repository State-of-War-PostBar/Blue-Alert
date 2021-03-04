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

#include "profile.h"

#include "../log/log.h"
#include "../type/generic.h"

#if defined SOWR_BUILD_DEBUG && defined SOWR_TARGET_WINDOWS
    static LARGE_INTEGER sowr_win_profile_timer_frequency;
#else
    #include <sys/time.h>

    typedef struct timeval sowr_PosixTime;
#endif

void
sowr_InitProfiler( void )
{
#if defined SOWR_BUILD_DEBUG && defined SOWR_TARGET_WINDOWS
    QueryPerformanceFrequency(&sowr_win_profile_timer_frequency);
#endif
}

void
sowr_ProfileFunc( const char *caller_file, const char *caller_func, int called_line )
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
            elapsed = (stop.QuadPart - start.QuadPart) * 1000.0 / sowr_win_profile_timer_frequency.QuadPart;
            SOWR_LOG_DEBUG_G(11,
                             SOWR_MAKE_GENERIC("Profiling "),
                             SOWR_MAKE_GENERIC(caller_func),
                             SOWR_MAKE_GENERIC(" (Line "),
                             SOWR_MAKE_GENERIC(start_line + 1),
                             SOWR_MAKE_GENERIC(" - "),
                             SOWR_MAKE_GENERIC(called_line - 1),
                             SOWR_MAKE_GENERIC(" in "),
                             SOWR_MAKE_GENERIC(caller_file),
                             SOWR_MAKE_GENERIC(") took "),
                             SOWR_MAKE_GENERIC_F(elapsed),
                             SOWR_MAKE_GENERIC(" ms.")
                            );
        }
    #else
        thread_local static sowr_PosixTime start, stop;

        if (first_called)
        {
            start_line = called_line;
            gettimeofday(&start, NULL);
        }
        else
        {
            gettimeofday(&stop, NULL);
            elapsed = (stop.tv_sec - start.tv_sec) * 1000.0f + (stop.tv_usec - start.tv_usec) / 1000.0f;
            SOWR_LOG_DEBUG_G(11,
                             SOWR_MAKE_GENERIC("Profiling "),
                             SOWR_MAKE_GENERIC(caller_func),
                             SOWR_MAKE_GENERIC(" (Line "),
                             SOWR_MAKE_GENERIC(start_line + 1),
                             SOWR_MAKE_GENERIC(" - "),
                             SOWR_MAKE_GENERIC(called_line - 1),
                             SOWR_MAKE_GENERIC(" in "),
                             SOWR_MAKE_GENERIC(caller_file),
                             SOWR_MAKE_GENERIC(") took "),
                             SOWR_MAKE_GENERIC_F(elapsed),
                             SOWR_MAKE_GENERIC(" ms.")
                            );
        }
    #endif
    first_called = !first_called;
#endif
}
