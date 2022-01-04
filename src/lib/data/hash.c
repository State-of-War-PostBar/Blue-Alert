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

#include "hash.h"

static const size_t SOWR_HASH_PRIME = 0x39bcbddd021;
static const size_t SOWR_HASH_OFFSET = 0x36fa2296f7439;
static const char SOWR_HASH_SALT[] = "tR+dK-naF*xenO/...";

sowr_Hash
sowr_GetHash( size_t length, const unsigned char *bytes )
{
    sowr_Hash res = SOWR_HASH_OFFSET;

    for (size_t i = 0; i < length; i++)
    {
        res ^= *bytes;
        res += SOWR_HASH_PRIME;
    }
    res *= SOWR_HASH_PRIME;
    for (size_t i = 0; i < sizeof(SOWR_HASH_SALT); i++)
    {
        res ^= SOWR_HASH_SALT[i];
        res *= SOWR_HASH_PRIME;
    }

    return res;
}

inline
sowr_Hash
sowr_GetHashI( const unsigned char *str )
{
    return sowr_GetHash(strlen((char *)str), str);
}

inline
sowr_Hash
sowr_GetHashS( const sowr_String *str )
{
    return sowr_GetHash(str->length, (unsigned char *)(str->ptr));
}
