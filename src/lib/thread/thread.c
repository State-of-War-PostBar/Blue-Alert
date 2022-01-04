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

#include "thread.h"

int
sowr_Thread_Create( sowr_Thread *thr, sowr_ThreadFunc func, void *arg )
{
#ifdef SOWR_TARGET_WINDOWS
    sowr_Thread thrd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, NULL);
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

sowr_Thread
sowr_Thread_Current( void )
{
#ifdef SOWR_TARGET_WINDOWS
    return GetCurrentThread();
#else
    return pthread_self();
#endif
}

void
sowr_Thread_Sleep( const struct timespec *duration )
{
#ifdef SOWR_TARGET_WINDOWS
    Sleep(duration->tv_sec * 1000.0 + duration->tv_nsec / 0.000001);
#else
    nanosleep(duration, NULL);
#endif
}

void
sowr_Thread_Exit( void )
{
#ifdef SOWR_TARGET_WINDOWS
    ExitThread(0);
#else
    pthread_exit(NULL);
#endif
}

void
sowr_Thread_Detach( sowr_Thread thr )
{
#ifdef SOWR_TARGET_WINDOWS
    CloseHandle(thr);
#else
    pthread_detach(thr);
#endif
}

void
sowr_Thread_Join( sowr_Thread thr )
{
#ifdef SOWR_TARGET_WINDOWS
    WaitForSingleObject(thr, INFINITE);
    CloseHandle(thr);
#else
    pthread_join(thr, NULL);
#endif
}
