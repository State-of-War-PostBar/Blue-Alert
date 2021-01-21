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

#ifndef SOWR_LIB_IO_FIO_H
#define SOWR_LIB_IO_FIO_H

#include <pch.h>

#ifdef SOWR_TARGET_WINDOWS
    typedef HANDLE sowr_FileDescriptor;

    #define SOWR_INVALID_FILE_DESCRIPTOR INVALID_HANDLE_VALUE
#else
    #include <dirent.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <unistd.h>

    typedef int sowr_FileDescriptor;

    #define SOWR_INVALID_FILE_DESCRIPTOR -1
#endif

typedef void (*sowr_DirWalkFunc)( const char * );

typedef enum sowr_FileWriteMode
{
    SOWR_FIO_WRITE_NORMAL,
    SOWR_FIO_WRITE_TRUNCATE,
    SOWR_FIO_WRITE_APPEND
} sowr_FileWriteMode;

///
/// \brief Open a file for reading
///
/// Open a file for reading.
///
/// \param path Path of the file
///
/// \return File descriptor of the opened file, or \a SOWR_INVALID_FILE_DESCRIPTOR if failed
///
sowr_FileDescriptor
sowr_File_OpenR( const char *path );

///
/// \brief Open a file for writing
///
/// Open a file for writing.
///
/// \param path Path of the file
/// \param mode Mode for writing
///
/// \return File descriptor of the opened file, or \a SOWR_INVALID_FILE_DESCRIPTOR if failed
///
sowr_FileDescriptor
sowr_File_OpenW( const char *path, sowr_FileWriteMode mode );

///
/// \brief Open a file for reading
///
/// Open a file for reading. If the file does not exist, it will be created.
///
/// \param path Path of the file
/// \param mode Mode for writing
///
/// \return File descriptor of the opened file
///
sowr_FileDescriptor
sowr_File_OpenOrCreate( const char *path, sowr_FileWriteMode mode );

///
/// \brief Close an opened file
///
/// Close an opened file descriptor.
///
/// \param file Opened file descriptor
///
void
sowr_File_Close( sowr_FileDescriptor file );

///
/// \brief Make directories
///
/// Make directories.<BR />
/// This function makes directories recursively.
///
/// \param path Path of the directories
///
void
sowr_File_Mkdir( const char *path );

///
/// \brief Walk a directory
///
/// Walk a directory and all its subdirectories.
///
/// \param path Path of the directory. Do not add a / at the end
/// \param func Function for walking
///
void
sowr_File_WalkDir( const char *path, sowr_DirWalkFunc func );

///
/// \brief Get the size of a file
///
/// Get the size of a file.
///
/// \param file File descriptor
///
/// \return Size of the file
///
size_t
sowr_File_GetSize( sowr_FileDescriptor file );

///
/// \brief Read the content of a file
///
/// Read the content of a file into a buffer.
///
/// \param file File to read
/// \param buffer Buffer to read into
/// \param sz Size in byte to read
///
/// \return If the reading succeeded
///
bool
sowr_File_ReadContent( sowr_FileDescriptor file, void *buffer, size_t sz );

///
/// \brief Write content to a file
///
/// Write the content of a buffer into a file.
///
/// \param file File to write into
/// \param buffer Buffer to write
/// \param sz Size in byte to write
///
/// \return If the writing succeeded
///
bool
sowr_File_WriteContent( sowr_FileDescriptor file, const void *buffer, size_t sz );

///
/// \brief Read file content
///
/// Read bytes content from a file path directly.
///
/// \param path Path of the file to read
///
/// \return The byte array of the file if reading successful, or NULL otherwise.<BR />
/// This array must be freed with \a sowr_HeapFree()
///
unsigned char *
sowr_FileEx_ReadContent( const char *path );

#endif // !SOWR_LIB_IO_FIO_H
