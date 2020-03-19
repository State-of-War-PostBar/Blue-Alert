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

#include "window.h"

static GLFWwindow *sowr_main_window;

static int sowr_window_width;
static int sowr_window_height;

static
inline
void
sowr_CenterMainWindow()
{
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(sowr_main_window, (mode->width - sowr_window_width) / 2, (mode->height - sowr_window_height) / 2);
}

void
sowr_CreateMainWindow(sowr_WindowCreateInfo *info)
{
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef SOWR_TARGET_MACOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    sowr_main_window = glfwCreateWindow(info->width, info->height, info->title, info->full_screen ? glfwGetPrimaryMonitor() : NULL, NULL);

    sowr_window_width = info->width;
    sowr_window_height = info->height;
    sowr_CenterMainWindow();
}

inline
void
sowr_HideWindow()
{
    glfwHideWindow(sowr_GetMainWindowContext());
}

inline
void
sowr_ShowWindow()
{
    glfwShowWindow(sowr_GetMainWindowContext());
}

inline
int
sowr_GetMainWindowWidth()
{
    return sowr_window_width;
}

inline
int
sowr_GetMainWindowHeight()
{
    return sowr_window_height;
}

inline
void
sowr_ResizeMainWindowWidth(int width)
{
    sowr_window_width = width;
    glfwSetWindowSize(sowr_main_window, sowr_window_width, sowr_window_height);
}

inline
void
sowr_ResizeMainWindowHeight(int height)
{
    sowr_window_height = height;
    glfwSetWindowSize(sowr_main_window, sowr_window_width, sowr_window_height);
}

inline
void
sowr_ResizeWindow(int width, int height)
{
    sowr_window_width = width;
    sowr_window_height = height;
    glfwSetWindowSize(sowr_main_window, width, height);
}

inline
void
sowr_MakeMainWindowCurrent()
{
    glfwMakeContextCurrent(sowr_main_window);
}

inline
GLFWwindow *
sowr_GetMainWindowContext()
{
    return sowr_main_window;
}

inline
bool
sowr_MainWindowShouldClose()
{
    return glfwWindowShouldClose(sowr_main_window);
}

inline
void
sowr_UpdateMainWindow()
{
    glfwSwapBuffers(sowr_main_window);
    glfwPollEvents();
}
