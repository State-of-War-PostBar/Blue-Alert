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

#ifndef BLRT_LIB_DATA_BYTES_H
#define BLRT_LIB_DATA_BYTES_H

#include <pch.h>

///
/// \brief Check endianness
///
/// Check the endianness of the system.
///
/// \return True if little endian, false if big endian
///
bool
blrt_IsLittleEndian( void );

///
/// \brief Swap two values
///
/// Swap two values. Two values should have the same size.
///
/// \param sz_l Size of the values
/// \param b_l Address of left op
/// \param b_r Address of right op
///
void
blrt_Swap( size_t sz, unsigned char *b_l, unsigned char *b_r );

#define BLRT_SWAP(l, r) blrt_Swap(sizeof(l) == sizeof(r) ? sizeof(l) : 0ULL, (unsigned char *) &(l), (unsigned char *) &(r))

///
/// \brief Swap endianness of some bytes
///
/// Swap the endianness of a sequence of bytes.
///
/// \param length Length of value in bytes
/// \param bytes Value to be swapped
///
void
blrt_SwapEndian( size_t length, unsigned char *bytes );

///
/// \brief Swap endianness of a 16-bit value
///
/// Swap the endianness of a 16-bit value.
///
/// \param val Value to be swapped
///
/// \return The swapped value
///
uint16_t
blrt_SwapEndian16( uint16_t val );

///
/// \brief Swap endianness of a 32-bit value
///
/// Swap the endianness of a 32-bit value.
///
/// \param val Value to be swapped
///
/// \return The swapped value
///
uint32_t
blrt_SwapEndian32( uint32_t val );

///
/// \brief Swap endianness of a 64-bit value
///
/// Swap the endianness of a 64-bit value.
///
/// \param val Value to be swapped
///
/// \return The swapped value
///
uint64_t
blrt_SwapEndian64( uint64_t val );

///
/// \brief Flip an atomic bool variable
///
/// Flip an atomic bool. True -> False, False -> True.
///
/// \param val Value to be flipped
///
void
blrt_FlipAtomicBool( atomic_bool *val );

uint32_t
blrt_RotateLeft32( uint32_t x, unsigned int n );

uint32_t
blrt_RotateRight32( uint32_t x, unsigned int n );

uint64_t
blrt_RotateLeft64( uint64_t x, unsigned int n );

uint64_t
blrt_RotateRight64( uint64_t x, unsigned int n );

#endif // !BLRT_LIB_DATA_BYTES_H
