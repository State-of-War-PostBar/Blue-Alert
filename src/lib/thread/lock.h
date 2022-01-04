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

#ifndef SOWR_LIB_THREAD_LOCK_H
#define SOWR_LIB_THREAD_LOCK_H

#include <pch.h>

#ifdef SOWR_TARGET_WINDOWS
    #include <synchapi.h>

    typedef HANDLE                  sowr_Mutex;
    typedef CRITICAL_SECTION        sowr_CriticalSection;

    typedef SECURITY_ATTRIBUTES     sowr_MutexAttribute;
#else
    #include <pthread.h>

    typedef pthread_mutex_t         sowr_Mutex;
    typedef pthread_mutex_t         sowr_CriticalSection;

    typedef pthread_mutexattr_t     sowr_MutexAttribute;
#endif

///
/// \brief Initialize a mutex
///
/// Initialize a mutual exclusion object. On Windows it would be WinMutex, or pthread's mutex otherwise.
/// 
/// \note It is recommanded to use critical sections on Windows instead of WinMutex for better performance.
///
/// \param mtx The mutex object to be initialized. <I>It must be allocated first!</I>
/// \param attr The mutex attribute for the mutex object
///
void
sowr_InitMutex( sowr_Mutex *mtx, sowr_MutexAttribute *attr );

///
/// \brief Lock a mutex
///
/// Attempt to lock a mutex object, waiting infinitely until the mutex is acquired.
///
/// \param mtx The mutex object to be locked
///
void
sowr_LockMutex( sowr_Mutex *mtx );

///
/// \brief Try to lock a mutex
///
/// Attempt to lock a mutex object, if the acquisition fails do nothing.
///
/// \param mtx The mutex object to be locked
///
void
sowr_TryLockMutex( sowr_Mutex *mtx );

///
/// \brief Unlock a mutex
///
/// Unlock a previously locked mutex acquired by this thread.
///
/// \param mtx The mutex object to be unlocked
///
void
sowr_UnlockMutex( sowr_Mutex *mtx );

///
/// \brief Destroy a mutex
///
/// Destroy a mutex object, it will be no longer usable.
///
/// \note This function does not deallocate the mutex object, you have to do it yourself if you wish so.
///
/// \param mtx The mutex object to be destroyed
///
void
sowr_DestroyMutex( sowr_Mutex *mtx );

///
/// \brief Initialize a critical section
///
/// Initialize a critical section, a lock on Windows that is way faster than WinMutex.<BR />
/// <B>If used on Posix systems, it has the same effect as \a sowr_InitMutex(), except that attributes cannot be applied to it.</B>
///
/// \param cs The critical section to be initialized. <I>It must be allocated first!</I>
///
void
sowr_InitCriticalSection( sowr_CriticalSection *cs );

///
/// \brief Enter a critical section
///
/// Enter <I>(that is, lock)</I> a critical section, waiting infinitely until entrance of the critical section.<BR />
/// <B>If used on Posix systems, it has the same effect as \a sowr_LockMutex().</B>
///
/// \param cs The critical section to be locked
///
void
sowr_EnterCriticalSection( sowr_CriticalSection *cs );

///
/// \brief Try to enter a critical section
///
/// Attempt to enter a critical section. If the entrance failed do nothing.<BR />
/// <B>If used on Posix systems, it has the same effect as \a sowr_TryLockMutex().</B>
///
/// \param cs The critical section to be locked
///
void
sowr_TryEnterCriticalSection( sowr_CriticalSection *cs );

///
/// \brief Leave a critical section
///
/// Leave a previously entered critical section.<BR />
/// <B>If used on Posix systems, it has the same effect as \a sowr_UnlockMutex().</B>
///
/// \param cs The critical section to be unlocked
/// 
void
sowr_LeaveCriticalSection( sowr_CriticalSection *cs );

///
/// \brief Destroy a critical section
///
/// Destroy a critical section, it will be no longer usable.<BR />
/// <B>If used on Posix systems, it has the same effect as \a sowr_DestroyMutex().</B>
///
/// \note This function does not deallocate the critical section, you have to do it yourself if you wish so.
///
/// \param cs The critical section to be destroyed
///
void
sowr_DestroyCriticalSection( sowr_CriticalSection *cs );

#endif // !SOWR_LIB_THREAD_LOCK_H
