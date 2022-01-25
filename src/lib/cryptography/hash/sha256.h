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

#ifndef BLRT_LIB_CRYPTOGRAPHY_HASH_SHA256_H
#define BLRT_LIB_CRYPTOGRAPHY_HASH_SHA256_H

#include <pch.h>

typedef union blrt_SHA256
{
    uint32_t word[8];
    uint8_t byte[32];
} blrt_SHA256;

///
/// \brief Get SHA-256
///
/// Get SHA-256 digest of a message.
///
/// \param length Length of the message
/// \param data Data to digest
///
/// \return SHA-256 hash of the message
///
blrt_SHA256
blrt_SHA256_Generate( uint64_t length, const unsigned char *data );

#endif // !BLRT_LIB_CRYPTOGRAPHY_HASH_SHA256_H
