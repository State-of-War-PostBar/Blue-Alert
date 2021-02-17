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

#include "mod.h"

#include "../lib/log/log.h"

sowr_Mod
sowr_Mod_Load(sowr_DL lib)
{
    rdk_ModMainFunc mod_main = (rdk_ModMainFunc) sowr_DL_Addr(lib, "rdk_ModMain");
    rdk_GetModInfoFunc mod_get_info = (rdk_GetModInfoFunc) sowr_DL_Addr(lib, "rdk_GetModInfo");
    rdk_ModExitFunc mod_exit = (rdk_ModExitFunc) sowr_DL_Addr(lib, "rdk_ModExit");
    if (!mod_get_info)
    {
        SOWR_LOG_ERROR("Loading a mod without rdk_GetModInfo function, failed to load!");
        return (sowr_Mod){ 0 };
    }

    if (!mod_main)
        SOWR_LOG_WARN("Loading a mod without rdk_ModMain function!");
    else
        mod_main();
    if (!mod_exit)
        SOWR_LOG_WARN("Loading a mod without rdk_ModExit function!");

    rdk_ModInfo mod_info = mod_get_info();
    return (sowr_Mod)
    {
        .mod_main = mod_main,
        .mod_get_info = mod_get_info,
        .mod_exit = mod_exit,
        .mod_info = mod_info
    };
}

void
sowr_Mod_Unload(sowr_Mod *mod)
{
    if (mod->mod_exit)
        mod->mod_exit();
}
