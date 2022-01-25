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

#ifndef BLRT_LIB_CRYPTOGRAPHY_CIPHER_ARS256_H
#define BLRT_LIB_CRYPTOGRAPHY_CIPHER_ARS256_H

#include <pch.h>

///
/// \brief Get length after padding
///
/// Get length of the message after padding with ISO/IEC 9797-1.
///
/// \param length Original length of the message
///
/// \return Length after padding
///
size_t
blrt_AES256_LengthAfterPadding( size_t length );

///
/// \brief Encrypt message
///
/// Encrypt message with AES-156.
///
/// \param length Length of the message
/// \param raw_data Data to be encrypted
/// \param key Key for encryption
/// \param encrypted_output Result of encryption. Should always be large enough to hold original message and padding
///
/// \return Length of the encrypted message
///
size_t
blrt_AES256_Encrypt( size_t length, const unsigned char *raw_data, const unsigned char key[32], unsigned char *encrypted_output );

///
/// \brief Decrypt message
///
/// Decrypt AES-256 encrypted message.
///
/// \param length Length of the encrypted message
/// \param encrypted_data Data to be decrypted
/// \param key Key for decryption
/// \param encrypted_output Result of encryption. Should always be at least as large as encrypted message
///
/// \return Length of the decrypted message without padding
///
size_t
blrt_AES256_Decrypt( size_t length, const unsigned char *encryted_data, const unsigned char key[32], unsigned char *decrypted_output );

#endif // !BLRT_LIB_CRYPTOGRAPHY_CIPHER_ARS256_H
