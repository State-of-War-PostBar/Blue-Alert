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

#include "../lib/log/log.h"
#include "../multimedia/window.h"
#include "../sync/multithread.h"

#include <glad/glad.h>
#include <pthread.h>

const char *const SOWR_PROG_ID                = "sowr";
const char *const SOWR_PROG_NAME              = "State of War: Remastered";

const char *const  SOWR_PROG_VERSION_STAGE    = "Indev";
const unsigned int SOWR_PROG_VERSION_MAJOR    = 0;
const unsigned int SOWR_PROG_VERSION_MINOR    = 0;
const unsigned int SOWR_PROG_VERSION_REVISION = 0;
const unsigned int SOWR_PROG_BUILD_NUMBER     = 1;

const char *const SOWR_LOG_FILE_NAME          = "sowr.log";

const unsigned int SOWR_INIT_WIN_WIDTH        = 1366;
const unsigned int SOWR_INIT_WIN_HEIGHT       = 768;

#ifdef SOWR_BUILD_DEBUG
    static bool sowr_log_available;
    static FILE *sowr_log_file;
    static sowr_CriticalSection sowr_log_file_mtx;

    static
    inline
    void
    sowr_LockLogFile(void *user_data, int lock)
    {
        lock ?
            sowr_EnterCriticalSection(&sowr_log_file_mtx)
        :
            sowr_LeaveCriticalSection(&sowr_log_file_mtx);
    }
#endif

void
sowr_InitLogger(void)
{
#ifdef SOWR_BUILD_DEBUG
    sowr_log_file = fopen(SOWR_LOG_FILE_NAME, "w");
    if (!sowr_log_file)
    {
        perror("Failed to create a log file, logging will not be availalbe.");
        sowr_log_available = false;
        return;
    }

    sowr_log_available = true;
    sowr_InitCriticalSection(&sowr_log_file_mtx);
    log_set_fp(sowr_log_file);
    log_set_level(SOWR_LOG_LEVEL_TRACE);
    log_set_lock(sowr_LockLogFile);
#endif
}

void
sowr_DestroyLogger(void)
{
#ifdef SOWR_BUILD_DEBUG
    if (sowr_log_available)
    {
        sowr_LockLogFile(NULL, true);
        fclose(sowr_log_file);
        sowr_LockLogFile(NULL, false);
        sowr_DestroyCriticalSection(&sowr_log_file_mtx);
    }
#endif
}

inline
void
sowr_InitGLFW(void)
{
    if (!glfwInit())
    {
        SOWR_LOG_ERROR("Failed to initialize GLFW.");
        abort();
    }
}

inline
void
sowr_DestroyGLFW(void)
{
    glfwTerminate();
}

inline
void
sowr_InitGLAD(void)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SOWR_LOG_ERROR("Failed to initialize GLAD.");
        abort();
    }
}

void
sowr_CreateProgramState(void)
{
    srand(time(NULL));
}

void
sowr_StartMainLoop(void)
{
    sowr_CenterMainWindow();
    glViewport(0, 0, sowr_g_window_width, sowr_g_window_height);

    while (!sowr_MainWindowShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.658f, 0.658f, 0.658f, 0.0f);

        sowr_UpdateMainWindow();
    }
}
