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

#ifndef SOWR_LIB_DATA_BYTES_H
#define SOWR_LIB_DATA_BYTES_H

#include <pch.h>

///
/// \brief Check endianness
///
/// Check the endianness of the system.
///
/// \return True if little endian, false if big endian
///
bool
sowr_Endianness( void );

///
/// \brief Swap endianness of some bytes
///
/// Swap the endianness of a sequence of bytes.
///
/// \param length Length of value in bytes
/// \param bytes Value to be swapped
///
void
sowr_SwapEndian( size_t length, unsigned char *bytes );

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
sowr_SwapEndian16( uint16_t val );

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
sowr_SwapEndian32( uint32_t val );

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
sowr_SwapEndian64( uint64_t val );

#endif // !SOWR_LIB_DATA_BYTES_H
