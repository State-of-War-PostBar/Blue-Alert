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

#ifndef BLRT_PCH_H
#define BLRT_PCH_H

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
#include <stdarg.h>
#include <stdatomic.h>
// #include <stdbit.h>                 -- Not yet available on most platforms.
#include <stdbool.h>
// #include <stdckdint.h>              -- Not yet available on most platforms.
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

// #include <threads.h>                -- Not yet available on most platforms.
#define thread_local _Thread_local

#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

#ifdef _WIN32
    #define BLRT_TARGET_WINDOWS
#elif defined __MACH__ && defined __APPLE__
    #define BLRT_TARGET_MACOS
    #define BLRT_TARGET_POSIX
#else
    #define BLRT_TARGET_POSIX
#endif

#ifdef _DEBUG
    #define BLRT_BUILD_DEBUG
#else
    #define BLRT_BUILD_RELEASE
#endif

#ifdef BLRT_TARGET_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#define BLRT_STRINGIFY(s) #s

#endif // !BLRT_PCH_H
