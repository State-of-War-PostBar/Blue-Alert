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

#ifndef RDK_H
#define RDK_H

// You may feel free to choose to include the the pch.h from sowr or not,
// respects to your convenience when using C standards.
// But if so, you have to include it before the next #ifndef!!!

#ifndef SOWR_PCH_H
    #if defined _MSC_VER || defined __MINGW32__
        #define SOWR_IMPORT __declspec(dllimport)
        #define SOWR_EXPORT __declspec(dllexport)
    #elif defined __GNUC__ || defined __clang__
        #define SOWR_IMPORT
        #define SOWR_EXPORT __attribute__((visibility("default")))
    #else
        #define SOWR_IMPORT
        #define SOWR_EXPORT
    #endif
#endif

typedef struct rdk_ModInfo
{
    const char *modid;
    const char *modname;
    const char *version;
} rdk_ModInfo;

///
/// \brief Mod initialize function
///
/// Initialize the mod. A mod will still be valid without this function defined.
///
SOWR_EXPORT
void
rdk_ModMain( void );

///
/// \brief Get the mod's info
///
/// Get the information about the mod. A mod must have this function and return a valid rdk_ModInfo struct.
///
/// \return The mod's mod information
///
SOWR_EXPORT
rdk_ModInfo
rdk_GetModInfo( void );

///
/// \brief Mod uninitialize function
///
/// Uninitialize the mod. A mod will still be valid without this function defined.
///
SOWR_EXPORT
void
rdk_ModExit( void );

#endif // !RDK_H
