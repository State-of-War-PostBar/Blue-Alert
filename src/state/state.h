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

#ifndef _SOWR_STATE_H_
#define _SOWR_STATE_H_

#include <pch.h>

/////////////////////////////////////////////////////
//////////////////   Definitions   //////////////////
/////////////////////////////////////////////////////

extern const char *          SOWR_PROG_ID;
extern const char *          SOWR_PROG_NAME;
extern const unsigned int    SOWR_PROG_VERSION_MAJOR;
extern const unsigned int    SOWR_PROG_VERSION_MINOR;
extern const unsigned int    SOWR_PROG_VERSION_REVISION;

extern const char *          SOWR_LOG_FILE_NAME;

extern const unsigned int    SOWR_INIT_WIN_WIDTH;
extern const unsigned int    SOWR_INIT_WIN_HEIGHT;

extern const unsigned int    SOWR_DEFAULT_LISTEN_PORT;

/////////////////////////////////////////////////
//////////////////   Logging   //////////////////
/////////////////////////////////////////////////

void
sowr_InitLogger();

void
sowr_DestroyLogger();

///////////////////////////////////////
///////////////// GL //////////////////
///////////////////////////////////////

void
sowr_InitGLFW();

void
sowr_DestroyGLFW();

void
sowr_InitGLAD();

///////////////////////////////////////////////
////////////////// Main Loop //////////////////
///////////////////////////////////////////////

void
sowr_StartMainLoop();

#endif // ! _SOWR_STATE_H_
