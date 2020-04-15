/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, some rights reserved.                              *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created, intended to be useful, but without any warranty.          *
* For more information, please forward to the license.                                           *
*                                                                                                *
* You should have received a copy of the license along with the source code of this              *
* program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.    *
*                                                                                                *
* Mission is successfully completed.                                                             *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_FILESYSTEM_FILESYSTEM_H
#define SOWR_LIB_FILESYSTEM_FILESYSTEM_H

#include <pch.h>

extern const char SOWR_PATH_SEP;

///
/// \brief Check if the path exists
///
/// Check if the path exists, works for both file and directory.
///
/// \param path Path to check
///
/// \return True if the path exists, false otherwise.
///
bool
sowr_PathExists(const char *path);

///
/// \brief Create a directory
///
/// Create a directory recursively.
///
/// \param path Directory to create
///
/// \return True if the directory is successfully created, false otherwise.
///
bool
sowr_CreateDirectory(const char *path);

///
///
/// \brief Get the file size
///
/// Get the size of a file in bytes.<BR />
/// This function is thread-safe but requires you to create file pointer yourself.
///
/// \param file File to get size
///
/// \return The file size
///
size_t
sowr_GetFileSize(FILE *file);

#endif
