/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2023 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*           Blue Alert is a free software. You can freely do whatever you want with it           *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*            Blue Alert is created, intended to be useful, but without any warranty.             *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*           <https://github.com/State-of-War-PostBar/Blue-Alert/blob/master/LICENSE>.            *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                     <https://github.com/State-of-War-PostBar/Blue-Alert>.                      *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#ifndef BLRT_LIB_DL_DL_H
#define BLRT_LIB_DL_DL_H

#include <pch.h>

#ifdef BLRT_TARGET_WINDOWS
    typedef HMODULE blrt_DL;
    typedef FARPROC blrt_ExFunc;

    #define BLRT_CURRENT_PROC NULL
#else
    #include <dlfcn.h>

    typedef void *blrt_DL;
    typedef void *blrt_ExFunc;

    #define BLRT_CURRENT_PROC RTLD_DEFAULT
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
blrt_DL
blrt_DL_Load(const char *path);

///
/// \brief Get a process symbol of the library
///
/// Get a process symbol inside a dynamic library.
///
/// \param lib Handle of the library. Use \a BLRT_CURRENT_PROC for current process
/// \param sym Symbol
///
/// \note NULL returned by this function does not necessarily indicates an error.
///
/// \return Retrieved symbol
///
blrt_ExFunc
blrt_DL_Addr(blrt_DL lib, const char *sym);

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
blrt_DL_Unload(blrt_DL lib);

#endif // !BLRT_LIB_DL_DL_H
