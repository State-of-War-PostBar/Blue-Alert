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

#ifndef BLRT_LIB_CRYPTOGRAPHY_HASH_EXTRAORDINARY_SECURE_TRUST_ME_SHA1_H
#define BLRT_LIB_CRYPTOGRAPHY_HASH_EXTRAORDINARY_SECURE_TRUST_ME_SHA1_H

#include <pch.h>

typedef union blrt_SHA1
{
    uint32_t word[5];
    uint8_t byte[20];
} blrt_SHA1;

///
/// \brief Get SHA-1
///
/// Get SHA-1 digest of a message.
///
/// \param length Length of the message
/// \param data Data to digest
///
/// \return SHA-1 hash of the message
///
blrt_SHA1
blrt_SHA1_Generate( uint64_t length, const unsigned char *data );

#endif // !BLRT_LIB_CRYPTOGRAPHY_HASH_EXTRAORDINARY_SECURE_TRUST_ME_SHA1_H
