/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (ɔ) 2017 - 2021 State of War Baidu Postbar, some rights reserved.                *
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
*                You should have received a copy of the license along with the                   *
*                       source code of this program. If not, please see                          *
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

#include "core.h"

#include "../lib/io/filesystem/fio.h"
#include "../lib/log/log.h"
#include "../lib/thread/lock.h"

const char *const    SOWR_PROG_ID          = "sowr";
const char *const    SOWR_PROG_NAME        = "State of War: Remastered";

const char *const    SOWR_VERSION_STAGE    = "Indev";
const unsigned int   SOWR_VERSION_MAJOR    = 0U;
const unsigned int   SOWR_VERSION_MINOR    = 1U;
const unsigned int   SOWR_VERSION_REVISION = 0U;
const char *const    SOWR_VERSION_STRING   = "Indev 0.1 rev0";

const unsigned int   SOWR_BUILD_NUMBER     = 1U;
const char *const    SOWR_BUILD_STRING     = "Build 1";

#ifdef SOWR_BUILD_DEBUG
    static const char *const SOWR_LOG_FILE_NAME = "./sowr.log";
    static bool sowr_log_available;
    static sowr_File sowr_log_file;
    static sowr_CriticalSection sowr_log_file_mtx;

    ///
    /// \brief Lock the log file
    ///
    /// Locking function for the log file, feed to logger.
    ///
    /// \param lock To lock or to unlock the log file. False for unlock
    ///
    static
    void
    sowr_LockLogFile( bool lock )
    {
        lock ?
            sowr_EnterCriticalSection(&sowr_log_file_mtx)
        :
            sowr_LeaveCriticalSection(&sowr_log_file_mtx);
    }
#endif

void
sowr_InitLogger( void )
{
#ifdef SOWR_BUILD_DEBUG
    sowr_log_file = sowr_File_OpenOrCreate(SOWR_LOG_FILE_NAME, SOWR_FIO_WRITE_TRUNCATE);
    if (!sowr_log_file)
    {
        perror("Failed to create a log file, logging will not be availalbe");
        sowr_log_available = false;
        return;
    }

    sowr_log_available = true;
    sowr_InitCriticalSection(&sowr_log_file_mtx);
    sowr_Logger_Init(sowr_log_file, sowr_LockLogFile);
#endif
}

void
sowr_DestroyLogger( void )
{
#ifdef SOWR_BUILD_DEBUG
    if (sowr_log_available)
    {
        sowr_Logger_Destroy();
        sowr_DestroyCriticalSection(&sowr_log_file_mtx);
        sowr_log_file = SOWR_INVALID_FILE_DESCRIPTOR;
        sowr_log_available = false;
    }
#endif
}
