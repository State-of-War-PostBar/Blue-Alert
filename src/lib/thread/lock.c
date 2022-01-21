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

#include "lock.h"

inline
void
blrt_InitMutex( blrt_Mutex *mtx, blrt_MutexAttribute *attr )
{
#ifdef BLRT_TARGET_WINDOWS
    *mtx = CreateMutex(attr, FALSE, NULL);
#else
    pthread_mutex_init(mtx, attr);
#endif
}

inline
void
blrt_LockMutex( blrt_Mutex *mtx )
{
#ifdef BLRT_TARGET_WINDOWS
    WaitForSingleObject(*mtx, INFINITE);
#else
    pthread_mutex_lock(mtx);
#endif
}

inline
void
blrt_TryLockMutex( blrt_Mutex *mtx )
{
#ifdef BLRT_TARGET_WINDOWS
    WaitForSingleObject(*mtx, 1L);
#else
    pthread_mutex_trylock(mtx);
#endif
}

inline
void
blrt_UnlockMutex( blrt_Mutex *mtx )
{
#ifdef BLRT_TARGET_WINDOWS
    ReleaseMutex(*mtx);
#else
    pthread_mutex_unlock(mtx);
#endif
}

inline
void
blrt_DestroyMutex( blrt_Mutex *mtx )
{
#ifdef BLRT_TARGET_WINDOWS
    CloseHandle(*mtx);
#else
    pthread_mutex_destroy(mtx);
#endif
}

inline
void
blrt_InitCriticalSection( blrt_CriticalSection *cs )
{
#ifdef BLRT_TARGET_WINDOWS
    InitializeCriticalSection(cs);
#else
    pthread_mutex_init(cs, NULL);
#endif
}

inline
void
blrt_EnterCriticalSection( blrt_CriticalSection *cs )
{
#ifdef BLRT_TARGET_WINDOWS
    EnterCriticalSection(cs);
#else
    pthread_mutex_lock(cs);
#endif
}

inline
void
blrt_TryEnterCriticalSection( blrt_CriticalSection *cs )
{
#ifdef BLRT_TARGET_WINDOWS
    TryEnterCriticalSection(cs);
#else
    pthread_mutex_trylock(cs);
#endif
}

inline
void
blrt_LeaveCriticalSection( blrt_CriticalSection *cs )
{
#ifdef BLRT_TARGET_WINDOWS
    LeaveCriticalSection(cs);
#else
    pthread_mutex_unlock(cs);
#endif
}

inline
void
blrt_DestroyCriticalSection( blrt_CriticalSection *cs )
{
#ifdef BLRT_TARGET_WINDOWS
    DeleteCriticalSection(cs);
#else
    pthread_mutex_destroy(cs);
#endif
}
