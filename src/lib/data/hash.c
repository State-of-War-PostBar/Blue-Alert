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

#include "hash.h"

static const size_t BLRT_HASH_PRIME = 0x39bcbddd021;
static const size_t BLRT_HASH_OFFSET = 0x36fa2296f7439;
static const char BLRT_HASH_SALT[] = "tR+dK-naF*xenO/...";

blrt_Hash
blrt_GetHash( size_t length, const unsigned char *bytes )
{
    blrt_Hash res = BLRT_HASH_OFFSET;

    for (size_t i = 0; i < length; i++)
    {
        res ^= *bytes;
        res += BLRT_HASH_PRIME;
    }
    res *= BLRT_HASH_PRIME;
    for (size_t i = 0; i < sizeof(BLRT_HASH_SALT); i++)
    {
        res ^= BLRT_HASH_SALT[i];
        res *= BLRT_HASH_PRIME;
    }

    return res;
}

inline
blrt_Hash
blrt_GetHashI( const unsigned char *str )
{
    return blrt_GetHash(strlen((char *)str), str);
}

inline
blrt_Hash
blrt_GetHashS( const blrt_String *str )
{
    return blrt_GetHash(str->length, (unsigned char *)(str->ptr));
}
