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

#ifndef _SOWR_WINDOW_H_
#define _SOWR_WINDOW_H_

#include <pch.h>

#include <glfw/glfw3.h>

typedef struct sowr_WindowCreateInfo_t
{
    int width;
    int height;
    const char *title;
    bool full_screen;
} sowr_WindowCreateInfo;

void
sowr_CreateMainWindow(sowr_WindowCreateInfo *);

void
sowr_HideWindow();

void
sowr_ShowWindow();

int
sowr_GetMainWindowWidth();

int
sowr_GetMainWindowHeight();

void
sowr_ResizeMainWindowWidth(int);

void
sowr_ResizeMainWindowHeight(int);

void
sowr_ResizeWindow(int, int);

void
sowr_MakeMainWindowCurrent();

GLFWwindow *
sowr_GetMainWindowContext();

bool
sowr_MainWindowShouldClose();

void
sowr_UpdateMainWindow();

#endif // ! _SOWR_WINDOW_H_
