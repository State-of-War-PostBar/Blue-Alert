/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, some rights reserved.                              *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created, intended to be useful, but without any warranty.          *
* For more information, please forward to the license.                                           *
*                                                                                                *
* You should have received a copy of the license along with the source code of this              *
* program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.    *
*                                                                                                *
* Mission is successfully completed.                                                             *
*                                                                                                *
**************************************************************************************************/

#include "state.h"

#include "../lib/log/log.h"
#include "../lib/sync/lock.h"

const char *const  SOWR_PROG_ID               = "sowr";
const char *const  SOWR_PROG_NAME             = "State of War: Remastered";

const char *const  SOWR_PROG_VERSION_STAGE    = "Indev";
const unsigned int SOWR_PROG_VERSION_MAJOR    = 0U;
const unsigned int SOWR_PROG_VERSION_MINOR    = 1U;
const unsigned int SOWR_PROG_VERSION_REVISION = 0U;
const char *const  SOWR_PROG_VERSION_STRING   = "Indev 0.1 rev0";

const unsigned int SOWR_PROG_BUILD_NUMBER     = 1U;
const char *const  SOWR_PROG_BUILD_STRING     = "Build 001";

#ifdef SOWR_BUILD_DEBUG
    static const char *const SOWR_LOG_FILE_NAME = "sowr.log";
    static bool sowr_log_available;
    static FILE *sowr_log_file;
    static sowr_CriticalSection sowr_log_file_mtx;

    ///
    /// \brief Lock the log file
    ///
    /// Locking function for the log file, feed to log.c.
    ///
    /// \param lock To lock or to unlock the log file. False for unlock.
    /// \param user_data User-defined data when called. Unused.
    ///
    static
    void
    sowr_LockLogFile( bool lock, void *user_data )
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
    sowr_log_file = fopen(SOWR_LOG_FILE_NAME, "w");
    if (!sowr_log_file)
    {
        perror("Failed to create a log file, logging will not be availalbe");
        sowr_log_available = false;
        return;
    }

    sowr_log_available = true;
    sowr_InitCriticalSection(&sowr_log_file_mtx);
    log_add_fp(sowr_log_file, SOWR_LOG_LEVEL_TRACE);
    log_set_lock(sowr_LockLogFile, NULL);
#endif
}

void
sowr_DestroyLogger( void )
{
#ifdef SOWR_BUILD_DEBUG
    if (sowr_log_available)
    {
        sowr_LockLogFile(true, NULL);
        fclose(sowr_log_file);
        sowr_LockLogFile(false, NULL);
        sowr_DestroyCriticalSection(&sowr_log_file_mtx);
        sowr_log_file = NULL;
        sowr_log_available = false;
    }
#endif
}
