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

#include "state.h"

#include "../common/log.h"
#include "../multimedia/window.h"
#include "../sync/multithread.h"

#include <glad/glad.h>
#include <pthread.h>

/////////////////////////////////////////////////////
//////////////////   Definitions   //////////////////
/////////////////////////////////////////////////////

const char *SOWR_PROG_ID                      = "keystel";
const char *SOWR_PROG_NAME                    = "Keystel";
const unsigned int SOWR_PROG_VERSION_MAJOR    = 0;
const unsigned int SOWR_PROG_VERSION_MINOR    = 0;
const unsigned int SOWR_PROG_VERSION_REVISION = 0;

const char *SOWR_LOG_FILE_NAME                = "keystel.log";

const unsigned int SOWR_INIT_WIN_WIDTH        = 1366;
const unsigned int SOWR_INIT_WIN_HEIGHT       = 768;

const unsigned int SOWR_DEFAULT_PORT          = 23333;

/////////////////////////////////////////////////
//////////////////   Logging   //////////////////
/////////////////////////////////////////////////

#ifdef SOWR_BUILD_DEBUG
    static bool sowr_log_available;
    static FILE *sowr_log_file;
    static sowr_Mutex sowr_log_file_mtx;
#endif

// I can actually remove the gap here, but it looks ugly so yeah.  -- Taxerap

#ifdef SOWR_BUILD_DEBUG
static inline
void
sowr_LockLogFile(void *_, int lock)
{
    lock ?
        sowr_LockMutex(&sowr_log_file_mtx)
    :
        sowr_UnlockMutex(&sowr_log_file_mtx);
}
#endif

void
sowr_InitLogger()
{
#ifdef SOWR_BUILD_DEBUG
    sowr_log_file = fopen(SOWR_LOG_FILE_NAME, "w");
    if (!sowr_log_file)
    {
        perror("Failed to create a log file, logging will not be availalbe.");
        return;
    }

    sowr_InitMutex(&sowr_log_file_mtx, NULL);
    log_set_fp(sowr_log_file);
    log_set_level(SOWR_LOG_LEVEL_TRACE);
    log_set_lock(sowr_LockLogFile);
    sowr_log_available = true;
#endif
}

void sowr_DestroyLogger()
{
#ifdef SOWR_BUILD_DEBUG
    if (sowr_log_available)
    {
        sowr_LockLogFile(NULL, true);
        fclose(sowr_log_file);
        sowr_LockLogFile(NULL, false);
        sowr_DestroyMutex(&sowr_log_file_mtx);
    }
#endif
}

///////////////////////////////////////
///////////////// GL //////////////////
///////////////////////////////////////

void
sowr_InitGLFW()
{
    if (!glfwInit())
    {
        SOWR_LOG_ERROR("Failed to initialize GLFW.");
        abort();
    }
}

void
sowr_DestroyGLFW()
{
    glfwTerminate();
}

void
sowr_InitGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        SOWR_LOG_ERROR("Failed to initialize GLAD.");
        abort();
    }
}

///////////////////////////////////////////////
////////////////// Main Loop //////////////////
///////////////////////////////////////////////

void
sowr_StartMainLoop()
{
    glViewport(0, 0, sowr_GetMainWindowWidth(), sowr_GetMainWindowHeight());

    while (!sowr_MainWindowShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.658f, 0.658f, 0.658f, 0.0f);

        sowr_UpdateMainWindow();
    }
}
