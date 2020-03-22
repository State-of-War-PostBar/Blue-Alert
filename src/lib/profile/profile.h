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

#ifndef SOWR_LIB_PROFILE_PROFILE_H
#define SOWR_LIB_PROFILE_PROFILE_H

#include <pch.h>

void
sowr_InitProfiler(void);

void
sowr_ProfileFunc(const char *, const char *, int);

#ifdef SOWR_BUILD_DEBUG
    #define SOWR_START_PROFILE() sowr_ProfileFunc(__FILE__, __func__, __LINE__)
    #define SOWR_STOP_PROFILE()  sowr_ProfileFunc(__FILE__, __func__, __LINE__)
#else
    #define SOWR_START_PROFILE()
    #define SOWR_STOP_PROFILE()
#endif

#endif // !SOWR_LIB_PROFILE_PROFILE_H
