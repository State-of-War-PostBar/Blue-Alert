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

#include "thread.h"

int
blrt_Thread_Create( blrt_Thread *thr, blrt_ThreadFunc func, void *arg )
{
#ifdef BLRT_TARGET_WINDOWS
    blrt_Thread thrd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) func, arg, 0, NULL);
    if (!thrd)
    {
        *thr = NULL;
        return -1;
    }
    *thr = thrd;
    return 0;
#else
    return pthread_create(thr, NULL, func, arg);
#endif
}

blrt_Thread
blrt_Thread_Current( void )
{
#ifdef BLRT_TARGET_WINDOWS
    return GetCurrentThread();
#else
    return pthread_self();
#endif
}

void
blrt_Thread_Sleep( const struct timespec *duration )
{
#ifdef BLRT_TARGET_WINDOWS
    Sleep(duration->tv_sec * 1000.0 + duration->tv_nsec / 0.000001);
#else
    nanosleep(duration, NULL);
#endif
}

void
blrt_Thread_Exit( void )
{
#ifdef BLRT_TARGET_WINDOWS
    ExitThread(0);
#else
    pthread_exit(NULL);
#endif
}

void
blrt_Thread_Detach( blrt_Thread thr )
{
#ifdef BLRT_TARGET_WINDOWS
    CloseHandle(thr);
#else
    pthread_detach(thr);
#endif
}

void
blrt_Thread_Join( blrt_Thread thr )
{
#ifdef BLRT_TARGET_WINDOWS
    WaitForSingleObject(thr, INFINITE);
    CloseHandle(thr);
#else
    pthread_join(thr, NULL);
#endif
}
