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

#ifndef SOWR_RDK_MOD_H
#define SOWR_RDK_MOD_H

#include <pch.h>

#include "rdk.h"
#include "../lib/dl/dl.h"

/*
    Let's wait for C2x's _typeof approval...
    See <http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2619.htm>

typedef _Typeof(rdk_ModMain) *rdk_ModMainFunc;
typedef _Typeof(rdk_GetModInfo) *rdk_GetModInfoFunc;
typedef _Typeof(rdk_ModExit) *rdk_ModExitFunc;

    _Typeof() should return a 'function' type instead of a 'function pointer' type when
    directly referncing to a function.
*/

typedef void (*rdk_ModMainFunc)(void);
typedef rdk_ModInfo (*rdk_GetModInfoFunc)(void);
typedef void (*rdk_ModExitFunc)(void);

typedef struct sowr_Mod
{
    rdk_ModMainFunc mod_main;
    rdk_GetModInfoFunc mod_get_info;
    rdk_ModExitFunc mod_exit;
    rdk_ModInfo mod_info;
} sowr_Mod;

///
/// \brief Load a mod
///
/// Load a mod.
///
/// \param lib Dynamic library of the mod
///
/// \return Loaded mod, or an empty struct if mod'd information cannot be retrieved
///
sowr_Mod
sowr_Mod_Load(sowr_DL lib);

///
/// \brief Unload a mod
///
/// Unload a mod.
///
/// \note This will not free the dynamic library loaded!
///
/// \param mod Mod to be unloaded
///
void
sowr_Mod_Unload(sowr_Mod *mod);

#endif // !SOWR_RDK_MOD_H
