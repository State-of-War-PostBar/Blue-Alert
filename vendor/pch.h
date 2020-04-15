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

#ifndef SOWR_PCH_H
#define SOWR_PCH_H

#if defined _WIN32 || (!defined _WIN32 && defined __CYGWIN__)
    #define SOWR_TARGET_WINDOWS
#elif defined __MACH__ && defined __APPLE__
    #define SOWR_TARGET_MACOS
    #define SOWR_TARGET_POSIX
#else
    #define SOWR_TARGET_POSIX
#endif

#ifdef _DEBUG
    #define SOWR_BUILD_DEBUG
#else
    #define SOWR_BUILD_RELEASE
#endif

#if -1 >> 1 == -1
    #define SOWR_ARCH_RSHIFT_RESERVE_SIGN
#endif

#include <assert.h>
#include <complex.h>
#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <tgmath.h>

#undef  thread_local
#define thread_local _Thread_local

#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

#ifdef SOWR_TARGET_WINDOWS
    #include <windows.h>
#endif

#endif // !SOWR_PCH_H
