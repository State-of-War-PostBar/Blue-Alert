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

#ifndef SOWR_LIB_THREAD_THREAD_H
#define SOWR_LIB_THREAD_THREAD_H

#include <pch.h>

#ifdef SOWR_TARGET_WINDOWS
    typedef HANDLE sowr_Thread;
#else
    #include <pthread.h>

    typedef pthread_t sowr_Thread;
#endif

typedef void (*sowr_ThreadFunc)( void * );

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
sowr_Thread_Create( sowr_Thread *thr, sowr_ThreadFunc func, void *arg );

///
/// \brief Get handle of current thread
///
/// Get the handle of the current thread
///
/// \return Thread handle
///
sowr_Thread
sowr_Thread_Current( void );

///
/// \brief Sleep the current thread
///
/// Pause the execution of current thread.
///
/// \param duration Time to sleep
///
void
sowr_Thread_Sleep( const struct timespec *duration );

///
/// \brief Yield the time slice
///
/// Yield the execution of current thread to others.
///
void
sowr_Thread_Yield( void );

///
/// \brief Exit the thread
///
/// Exit current thread.
///
void
sowr_Thread_Exit( void );

///
/// \brief Detach a thread
///
/// Detach a thread from current thread.
///
/// \param thr Thread to detach
///
void
sowr_Thread_Detach( sowr_Thread thr );

///
/// \brief Join a thread
///
/// Block current thread until the thread ends.
///
/// \param thr Thread to join
///
void
sowr_Thread_Join( sowr_Thread thr );

#endif // !SOWR_LIB_THREAD_THREAD_H
