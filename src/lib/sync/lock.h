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

#ifndef SOWR_LIB_SYNC_LOCK_H
#define SOWR_LIB_SYNC_LOCK_H

#include <pch.h>

#ifdef SOWR_TARGET_WINDOWS
    #include <synchapi.h>

    typedef HANDLE                  sowr_Mutex;
    typedef CRITICAL_SECTION        sowr_CriticalSection;
    typedef SRWLOCK                 sowr_ReadWriteLock;

    typedef SECURITY_ATTRIBUTES     sowr_MutexAttribute;
#else
    #include <pthread.h>

    typedef pthread_mutex_t         sowr_Mutex;
    typedef pthread_mutex_t         sowr_CriticalSection;
    typedef pthread_rwlock_t        sowr_ReadWriteLock;

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

///
/// \deprecated It sucks.
///
/// \brief Initialize a read-write lock
///
/// Initialize a read-write lock.
///
/// \note On Windows, it has some weird compatibility because of SysWOW64. Try not to use it.
///
/// \param rwl The read-write lock to be initialized. <I>It must be allocated first!</I>
///
void
sowr_InitReadWriteLock( sowr_ReadWriteLock *rwl );

///
/// \deprecated It sucks.
///
/// \brief Lock a read-write lock in read mode
///
/// Lock a read-write lock in read mode, waiting infinitely until the acquisition.<BR />
/// Multiple readers can lock it in read mode an the same time, but no reader or writer
/// can obtain the read-write lock if there is one writer acquired it.
///
/// \note On Windows, it has some weird compatibility because of SysWOW64. Try not to use it.
///
/// \param rwl The read-write lock to be locked
///
void
sowr_LockReadWriteLockRead( sowr_ReadWriteLock *rwl );

///
/// \deprecated It sucks.
///
/// \brief Lock a read-write lock in write mode
///
/// Lock a read-write lock in write mode, waiting infinitely until the acquisition.<BR />
/// No other reader or writer can obtain the read-write lock after the writer acquired it.
///
/// \note On Windows, it has some weird compatibility because of SysWOW64. Try not to use it.
///
/// \param rwl The read-write lock to be locked
///
void
sowr_LockReadWriteLockWrite( sowr_ReadWriteLock *rwl );

///
/// \deprecated It sucks.
///
/// \brief Lock a read-write lock in read mode
///
/// Lock a read-write lock in read mode, if the acquisition fails do nothing.<BR />
/// Multiple readers can lock it in read mode an the same time, but no reader or writer
/// can obtain the read-write lock if there is one writer acquired it.
///
/// \note On Windows, it has some weird compatibility because of SysWOW64. Try not to use it.
///
/// \param rwl The read-write lock to be locked
///
void
sowr_TryLockReadWriteLockRead( sowr_ReadWriteLock *rwl );

///
/// \deprecated It sucks.
///
/// \brief Lock a read-write lock in write mode
///
/// Lock a read-write lock in write mode, if the acquisition fails continue immediately.<BR />
/// No other reader or writer can obtain the read-write lock after the writer acquired it.
///
/// \note On Windows, it has some weird compatibility because of SysWOW64. Try not to use it.
///
/// \param rwl The read-write lock to be locked
///
void
sowr_TryLockReadWriteLockWrite( sowr_ReadWriteLock *rwl );

///
/// \deprecated It sucks.
///
/// \brief Unlock a read mode read-write lock
///
/// Unlock a reader mode read-write lock. It must be the reader to unlock.
///
/// \note On Windows, it has some weird compatibility because of SysWOW64. Try not to use it.
/// \note On Posix, it has the same effect as \a sowr_UnlockReadWriteLockWrite() since you don't have to distinguish them when unlocking.
///
/// \param rwl The read-write lock to be unlocked
///
void
sowr_UnlockReadWriteLockRead( sowr_ReadWriteLock *rwl );

///
/// \deprecated It sucks.
///
/// \brief Unlock a write mode read-write lock
///
/// Unlock a writer mode read-write lock. It must be the writer to unlock.
///
/// \note On Windows, it has some weird compatibility because of SysWOW64. Try not to use it.
/// \note On Posix, it has the same effect as \a sowr_UnlockReadWriteLockRead() since you don't have to distinguish them when unlocking.
///
/// \param rwl The read-write lock to be unlocked
///
void
sowr_UnlockReadWriteLockWrite( sowr_ReadWriteLock *rwl );

///
/// \deprecated It sucks.
///
/// \brief Destroy a read-write lock
///
/// Destroy a read-write lock, it will be no longer usable.
///
/// \note On Windows, this function does nothing. Also it has some weird compatibility because of SysWOW64. Try not to use it.
///
/// \param rwl The read-write lock to be destroyed
///
void
sowr_DestroyReadWriteLock( sowr_ReadWriteLock *rwl );

#endif // !SOWR_LIB_SYNC_LOCK_H
