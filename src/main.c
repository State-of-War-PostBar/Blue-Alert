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

#include <pch.h>

#include "common/log.h"
#include "common/profile.h"
#include "data/data.h"
#include "multimedia/window.h"
#include "state/state.h"

int
main(int argc, char *argv[])
{
#if defined SOWR_BUILD_RELEASE && defined SOWR_TARGET_WINDOWS
    FreeConsole();
#endif
    sowr_InitLogger();
    sowr_InitProfiler();
    SOWR_LOG_INFO("Starting...");
    SOWR_LOG_INFO("Really starting...");

    sowr_WindowCreateInfo info =
    {
        .width = SOWR_INIT_WIN_WIDTH,
        .height = SOWR_INIT_WIN_HEIGHT,
        .title = SOWR_PROG_NAME,
        .full_screen = false
    };

    sowr_InitGLFW();
    sowr_CreateMainWindow(&info);

    sowr_ShowWindow();

    sowr_MakeMainWindowCurrent();
    sowr_InitGLAD();

    sowr_StartMainLoop();

    sowr_DestroyGLFW();

    SOWR_LOG_INFO("Bye.");
    sowr_DestroyLogger();
    return 0;
}
