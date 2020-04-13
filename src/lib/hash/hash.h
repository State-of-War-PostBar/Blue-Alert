/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, all rights reserved.                               *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created intended to be useful, but without any warranty.           *
* For more information, please forward to the license.                                           *
*                                                                                                *
* You should have received a copy of the license along with the source code of this              *
* program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.    *
*                                                                                                *
* Mission is successfully completed.                                                             *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_HASH_HASH_H
#define SOWR_LIB_HASH_HASH_H

#include <pch.h>

///
/// Hashed value
///
typedef size_t sowr_HashVal;

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
sowr_HashVal
sowr_GetHash(size_t length, const char *bytes);

///
/// \brief Hash a string
///
/// Get the hashcode of a string.
/// The string is understood to be null-terminated
///
/// \param str String to hash
///
/// \return Hashed result
///
sowr_HashVal
sowr_GetHashS(const char *str);

#endif // !SOWR_LIB_HASH_HASH_H
