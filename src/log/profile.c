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

#include "profile.h"

#include "log.h"
#include "../lib/type/generic.h"

#if defined BLRT_BUILD_DEBUG && defined BLRT_TARGET_WINDOWS
    static LARGE_INTEGER blrt_win_profile_timer_frequency;
#else
    #include <sys/time.h>

    typedef struct timeval blrt_PosixTime;
#endif

void
blrt_InitProfiler( void )
{
#if defined BLRT_BUILD_DEBUG && defined BLRT_TARGET_WINDOWS
    QueryPerformanceFrequency(&blrt_win_profile_timer_frequency);
#endif
}

void
blrt_ProfileFunc( const char *caller_file, const char *caller_func, int called_line )
{
#ifdef BLRT_BUILD_DEBUG
    thread_local static double elapsed;
    thread_local static bool first_called = true;
    thread_local static int start_line;

    #ifdef BLRT_TARGET_WINDOWS
        thread_local static LARGE_INTEGER start, stop;

        if (first_called)
        {
            start_line = called_line;
            QueryPerformanceCounter(&start);
        }
        else
        {
            QueryPerformanceCounter(&stop);
            elapsed = (stop.QuadPart - start.QuadPart) * 1000.0 / blrt_win_profile_timer_frequency.QuadPart;
            BLRT_LOG_DEBUG_G(11,
                             BLRT_MAKE_GENERIC("Profiling "),
                             BLRT_MAKE_GENERIC(caller_func),
                             BLRT_MAKE_GENERIC(" (Line "),
                             BLRT_MAKE_GENERIC(start_line + 1),
                             BLRT_MAKE_GENERIC(" - "),
                             BLRT_MAKE_GENERIC(called_line - 1),
                             BLRT_MAKE_GENERIC(" in "),
                             BLRT_MAKE_GENERIC(caller_file),
                             BLRT_MAKE_GENERIC(") took "),
                             BLRT_MAKE_GENERIC_F(elapsed),
                             BLRT_MAKE_GENERIC(" ms.")
                            );
        }
    #else
        thread_local static blrt_PosixTime start, stop;

        if (first_called)
        {
            start_line = called_line;
            gettimeofday(&start, NULL);
        }
        else
        {
            gettimeofday(&stop, NULL);
            elapsed = (stop.tv_sec - start.tv_sec) * 1000.0f + (stop.tv_usec - start.tv_usec) / 1000.0f;
            BLRT_LOG_DEBUG_G(11,
                             BLRT_MAKE_GENERIC("Profiling "),
                             BLRT_MAKE_GENERIC(caller_func),
                             BLRT_MAKE_GENERIC(" (Line "),
                             BLRT_MAKE_GENERIC(start_line + 1),
                             BLRT_MAKE_GENERIC(" - "),
                             BLRT_MAKE_GENERIC(called_line - 1),
                             BLRT_MAKE_GENERIC(" in "),
                             BLRT_MAKE_GENERIC(caller_file),
                             BLRT_MAKE_GENERIC(") took "),
                             BLRT_MAKE_GENERIC_F(elapsed),
                             BLRT_MAKE_GENERIC(" ms.")
                            );
        }
    #endif
    first_called = !first_called;
#endif
}
