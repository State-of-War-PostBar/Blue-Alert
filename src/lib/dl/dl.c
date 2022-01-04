/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
*                  any later version of the document released by mhtvsSFrpHdE.                   *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*              <https://github.com/State-of-War-PostBar/sowr/blob/master/LICENSE>.               *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                        <https://github.com/State-of-War-PostBar/sowr>.                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#include "dl.h"

#include "../container/vector.h"
#include "../data/unicode.h"

sowr_DL
sowr_DL_Load(const char *path)
{
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Unicode_UTF8ToUTF16((unsigned char *) path, &utf16);
    sowr_DL dl = LoadLibraryW(utf16.ptr);
    sowr_Vector_DestroyS(&utf16);
    return dl;
#else
    return dlopen(path, RTLD_LAZY);
#endif
}

sowr_ExFunc
sowr_DL_Addr(sowr_DL lib, const char *sym)
{
#ifdef SOWR_TARGET_WINDOWS
    return GetProcAddress(lib, sym);
#else
    return dlsym(lib, sym);
#endif
}

int
sowr_DL_Unload(sowr_DL lib)
{
#ifdef SOWR_TARGET_WINDOWS
    return FreeLibrary(lib);
#else
    return dlclose(lib);
#endif
}
