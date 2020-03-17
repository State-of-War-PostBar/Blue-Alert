/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, all rights reserved.                               *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created intended to be useful, but without any warranty.           *
* For more information, please forward to the license.                                           *
*                                                                                                *
* You should have received a copy of the license along with the source code of this              *
* program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.    *
*                                                                                                *
* Mission is successfully completed.                                                             *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_SYNC_MULTITHREAD_H
#define SOWR_SYNC_MULTITHREAD_H

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

void
sowr_InitMutex(sowr_Mutex *, sowr_MutexAttribute *);

void
sowr_LockMutex(sowr_Mutex *);

void
sowr_TryLockMutex(sowr_Mutex *);

void
sowr_UnlockMutex(sowr_Mutex *);

void
sowr_DestroyMutex(sowr_Mutex *);

void
sowr_InitCriticalSection(sowr_CriticalSection *);

void
sowr_EnterCriticalSection(sowr_CriticalSection *);

void
sowr_TryEnterCriticalSection(sowr_CriticalSection *);

void
sowr_LeaveCriticalSection(sowr_CriticalSection *);

void
sowr_DestroyCriticalSection(sowr_CriticalSection *);

void
sowr_InitReadWriteLock(sowr_ReadWriteLock *);

void
sowr_LockReadWriteLockRead(sowr_ReadWriteLock *);

void
sowr_LockReadWriteLockWrite(sowr_ReadWriteLock *);

void
sowr_TryLockReadWriteLockRead(sowr_ReadWriteLock *);

void
sowr_TryLockReadWriteLockWrite(sowr_ReadWriteLock *);

void
sowr_UnlockReadWriteLockRead(sowr_ReadWriteLock *);

void
sowr_UnlockReadWriteLockWrite(sowr_ReadWriteLock *);

void
sowr_DestroyReadWriteLock(sowr_ReadWriteLock *);

#endif // !SOWR_SYNC_MULTITHREAD_H
