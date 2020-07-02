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

#ifndef SOWR_CORE_CORE_H
#define SOWR_CORE_CORE_H

#include <pch.h>

extern const char *const     SOWR_PROG_ID;
extern const char *const     SOWR_PROG_NAME;

extern const char *const     SOWR_PROG_VERSION_STAGE;
extern const unsigned int    SOWR_PROG_VERSION_MAJOR;
extern const unsigned int    SOWR_PROG_VERSION_MINOR;
extern const unsigned int    SOWR_PROG_VERSION_REVISION;
extern const char *const     SOWR_PROG_VERSION_STRING;

extern const unsigned int    SOWR_PROG_BUILD_NUMBER;
extern const char *const     SOWR_PROG_BUILD_STRING;

///
/// \brief Initialize the logger
///
/// Initialize the logger of program. This must be called for using logging feature.
///
void
sowr_InitLogger( void );

///
/// \brief Destroy the logger
///
/// Destroy the logger and release all resources.
///
void
sowr_DestroyLogger( void );

#endif // !SOWR_CORE_CORE_H
