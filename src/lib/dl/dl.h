/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2021 State of War Baidu Postbar, some rights reserved.                *
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
*       You should have received a copy of the license along with the source code of this        *
*  program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.   *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                         https://github.com/State-of-War-PostBar/sowr.                          *
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_DL_DL_H
#define SOWR_LIB_DL_DL_H

#include <pch.h>

#ifdef SOWR_TARGET_WINDOWS
    typedef HMODULE sowr_DL;
    typedef FARPROC sowr_ExFunc;

    #define SOWR_CURRENT_PROC NULL
#else
    #include <dlfcn.h>

    typedef void *sowr_DL;
    typedef void *sowr_ExFunc;

    #define SOWR_CURRENT_PROC RTLD_DEFAULT
#endif

///
/// \brief Load a library
///
/// Load a dynamic library.
///
/// \param path Path to the library file
///
/// \return Handle of the library, or NULL if errors occur
///
sowr_DL
sowr_DL_Load(const char *path);

///
/// \brief Get a process symbol of the library
///
/// Get a process symbol inside a dynamic library.
///
/// \param lib Handle of the library. Use \a SOWR_CURRENT_PROC for current process
/// \param sym Symbol
///
/// \note NULL returned by this function does not necessarily indicates an error.
///
/// \return Retrieved symbol
///
sowr_ExFunc
sowr_DL_Addr(sowr_DL lib, const char *sym);

///
/// \brief Free a library
///
/// Free a dynamic library.
///
/// \param lib Handle of the library
///
/// \return 0 if success
///
int
sowr_DL_Unload(sowr_DL lib);

#endif // !SOWR_LIB_DL_DL_H
