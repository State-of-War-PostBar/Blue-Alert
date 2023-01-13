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

#ifndef BLRT_LIB_DATA_HASH_H
#define BLRT_LIB_DATA_HASH_H

#include <pch.h>

#include "../container/string.h"

/// Hashed value
typedef size_t blrt_Hash;

///
/// \brief Hash some bytes
///
/// Get the hashcode of some bytes.
///
/// \param length Length of the byte array
/// \param bytes Byte array to hash
///
/// \return Hashed result
///
blrt_Hash
blrt_GetHash( size_t length, const unsigned char *bytes );

///
/// \brief Hash a string
///
/// Get the hashcode of a string.<BR />
/// The string is understood to be null-terminated.
///
/// \param str String to hash
///
/// \return Hashed result
///
blrt_Hash
blrt_GetHashI( const unsigned char *str );

///
/// \brief Hash a string
///
/// Get the hashcode of a string.<BR />
///
/// \param str String to hash
///
/// \return Hashed result
///
blrt_Hash
blrt_GetHashS( const blrt_String *str );

#endif // !BLRT_LIB_DATA_HASH_H
