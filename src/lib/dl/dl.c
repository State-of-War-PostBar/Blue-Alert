/*************************************************************************************************
*                                                                                                *
*                                         [ Blue Alert ]                                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                              A free, open-source indie RTS game.                               *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
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

#include "dl.h"

#include "../container/vector.h"
#include "../data/unicode.h"

blrt_DL
blrt_DL_Load(const char *path)
{
#ifdef BLRT_TARGET_WINDOWS
    blrt_Vector utf16 = blrt_Vector_CreateS(sizeof(unsigned char), NULL);
    blrt_Unicode_UTF8ToUTF16((unsigned char *) path, &utf16);
    blrt_DL dl = LoadLibraryW(utf16.ptr);
    blrt_Vector_DestroyS(&utf16);
    return dl;
#else
    return dlopen(path, RTLD_LAZY);
#endif
}

blrt_ExFunc
blrt_DL_Addr(blrt_DL lib, const char *sym)
{
#ifdef BLRT_TARGET_WINDOWS
    return GetProcAddress(lib, sym);
#else
    return dlsym(lib, sym);
#endif
}

int
blrt_DL_Unload(blrt_DL lib)
{
#ifdef BLRT_TARGET_WINDOWS
    return FreeLibrary(lib);
#else
    return dlclose(lib);
#endif
}
