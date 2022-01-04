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

#include "lock.h"

inline
void
sowr_InitMutex( sowr_Mutex *mtx, sowr_MutexAttribute *attr )
{
#ifdef SOWR_TARGET_WINDOWS
    *mtx = CreateMutex(attr, FALSE, NULL);
#else
    pthread_mutex_init(mtx, attr);
#endif
}

inline
void
sowr_LockMutex( sowr_Mutex *mtx )
{
#ifdef SOWR_TARGET_WINDOWS
    WaitForSingleObject(*mtx, INFINITE);
#else
    pthread_mutex_lock(mtx);
#endif
}

inline
void
sowr_TryLockMutex( sowr_Mutex *mtx )
{
#ifdef SOWR_TARGET_WINDOWS
    WaitForSingleObject(*mtx, 1L);
#else
    pthread_mutex_trylock(mtx);
#endif
}

inline
void
sowr_UnlockMutex( sowr_Mutex *mtx )
{
#ifdef SOWR_TARGET_WINDOWS
    ReleaseMutex(*mtx);
#else
    pthread_mutex_unlock(mtx);
#endif
}

inline
void
sowr_DestroyMutex( sowr_Mutex *mtx )
{
#ifdef SOWR_TARGET_WINDOWS
    CloseHandle(*mtx);
#else
    pthread_mutex_destroy(mtx);
#endif
}

inline
void
sowr_InitCriticalSection( sowr_CriticalSection *cs )
{
#ifdef SOWR_TARGET_WINDOWS
    InitializeCriticalSection(cs);
#else
    pthread_mutex_init(cs, NULL);
#endif
}

inline
void
sowr_EnterCriticalSection( sowr_CriticalSection *cs )
{
#ifdef SOWR_TARGET_WINDOWS
    EnterCriticalSection(cs);
#else
    pthread_mutex_lock(cs);
#endif
}

inline
void
sowr_TryEnterCriticalSection( sowr_CriticalSection *cs )
{
#ifdef SOWR_TARGET_WINDOWS
    TryEnterCriticalSection(cs);
#else
    pthread_mutex_trylock(cs);
#endif
}

inline
void
sowr_LeaveCriticalSection( sowr_CriticalSection *cs )
{
#ifdef SOWR_TARGET_WINDOWS
    LeaveCriticalSection(cs);
#else
    pthread_mutex_unlock(cs);
#endif
}

inline
void
sowr_DestroyCriticalSection( sowr_CriticalSection *cs )
{
#ifdef SOWR_TARGET_WINDOWS
    DeleteCriticalSection(cs);
#else
    pthread_mutex_destroy(cs);
#endif
}
