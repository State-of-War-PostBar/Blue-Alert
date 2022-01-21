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

#ifndef BLRT_LIB_THREAD_THREAD_H
#define BLRT_LIB_THREAD_THREAD_H

#include <pch.h>

#ifdef BLRT_TARGET_WINDOWS
    typedef HANDLE blrt_Thread;
#else
    #include <pthread.h>

    typedef pthread_t blrt_Thread;
#endif

typedef void (*blrt_ThreadFunc)( void * );

///
/// \brief Create a thread
///
/// Create a thread and run it immediately.
///
/// \param thr Pointer to a storage of the thread handle
/// \param func Function to run
/// \param arg argument to the function
///
/// \return 0 if successed
///
int
blrt_Thread_Create( blrt_Thread *thr, blrt_ThreadFunc func, void *arg );

///
/// \brief Get handle of current thread
///
/// Get the handle of the current thread
///
/// \return Thread handle
///
blrt_Thread
blrt_Thread_Current( void );

///
/// \brief Sleep the current thread
///
/// Pause the execution of current thread.
///
/// \param duration Time to sleep
///
void
blrt_Thread_Sleep( const struct timespec *duration );

///
/// \brief Exit the thread
///
/// Exit current thread.
///
void
blrt_Thread_Exit( void );

///
/// \brief Detach a thread
///
/// Detach a thread from current thread.
///
/// \param thr Thread to detach
///
void
blrt_Thread_Detach( blrt_Thread thr );

///
/// \brief Join a thread
///
/// Block current thread until the thread ends.
///
/// \param thr Thread to join
///
void
blrt_Thread_Join( blrt_Thread thr );

#endif // !BLRT_LIB_THREAD_THREAD_H
