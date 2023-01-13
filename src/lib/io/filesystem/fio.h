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

#ifndef BLRT_LIB_IO_FILESYSTEM_FIO_H
#define BLRT_LIB_IO_FILESYSTEM_FIO_H

#include <pch.h>

#ifdef BLRT_TARGET_WINDOWS
    typedef HANDLE blrt_File;

    #define BLRT_INVALID_FILE_DESCRIPTOR INVALID_HANDLE_VALUE

    #define BLRT_FILE_STDIN  STD_INPUT_HANDLE
    #define BLRT_FILE_STDOUT STD_OUTPUT_HANDLE
    #define BLRT_FILE_STDERR STD_ERROR_HANDLE
#else
    #include <dirent.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <unistd.h>

    typedef int blrt_File;

    #define BLRT_INVALID_FILE_DESCRIPTOR -1

    #define BLRT_FILE_STDIN  STDIN_FILENO
    #define BLRT_FILE_STDOUT STDOUT_FILENO
    #define BLRT_FILE_STDERR STDERR_FILENO
#endif

typedef void (*blrt_DirWalkFunc)( const char * );

typedef enum blrt_FileWriteMode
{
    BLRT_FIO_WRITE_NORMAL,
    BLRT_FIO_WRITE_TRUNCATE,
    BLRT_FIO_WRITE_APPEND
} blrt_FileWriteMode;

///
/// \brief Open a file for reading
///
/// Open a file for reading.
///
/// \param path Path of the file
///
/// \return File descriptor of the opened file, or \a BLRT_INVALID_FILE_DESCRIPTOR if failed
///
blrt_File
blrt_File_OpenR( const char *path );

///
/// \brief Open a file for writing
///
/// Open a file for writing.
///
/// \param path Path of the file
/// \param mode Mode for writing
///
/// \return File descriptor of the opened file, or \a BLRT_INVALID_FILE_DESCRIPTOR if failed
///
blrt_File
blrt_File_OpenW( const char *path, blrt_FileWriteMode mode );

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
blrt_File
blrt_File_OpenOrCreate( const char *path, blrt_FileWriteMode mode );

///
/// \brief Close an opened file
///
/// Close an opened file descriptor.
///
/// \param file Opened file descriptor
///
void
blrt_File_Close( blrt_File file );

///
/// \brief Make directories
///
/// Make directories.<BR />
/// This function makes directories recursively.
///
/// \param path Path of the directories
///
void
blrt_File_Mkdir( const char *path );

///
/// \brief Walk a directory
///
/// Walk a directory and all its subdirectories.
///
/// \param path Path of the directory. Do not add a / at the end
/// \param func Function for walking
///
void
blrt_File_WalkDir( const char *path, blrt_DirWalkFunc func );

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
blrt_File_GetSize( blrt_File file );

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
blrt_File_ReadContent( blrt_File file, void *buffer, size_t sz );

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
blrt_File_WriteContent( blrt_File file, const void *buffer, size_t sz );

///
/// \brief Write contents to a file
///
/// Write a series of contents into a file.
///
/// \param file File to write into
/// \param count Parameters that needs to be written
///
void
blrt_File_WriteContents( blrt_File file, size_t count, ... );

///
/// \brief Write contents to a file
///
/// Write a series of contents into a file.
///
/// \param file File to write into
/// \param count Parameters that needs to be written
/// \param args Arguments for writing
///
void
blrt_File_WriteContentsV( blrt_File file, size_t count, va_list *args );

#endif // !BLRT_LIB_IO_FILESYSTEM_FIO_H
