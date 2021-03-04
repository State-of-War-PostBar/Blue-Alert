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

#include "fio.h"

#include "../../container/string.h"
#include "../../data/unicode.h"
#include "../../memory/heap_memory.h"
#include "../../type/generic.h"

sowr_File
sowr_File_OpenR( const char *path )
{
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
    sowr_File hdl = CreateFileW((wchar_t *)(utf16.ptr), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    sowr_Vector_DestroyS(&utf16);
    return hdl;
#else
    return open(path, O_RDONLY);
#endif
}

sowr_File
sowr_File_OpenW( const char *path, sowr_FileWriteMode mode )
{
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
    sowr_File hdl = CreateFileW((wchar_t *)(utf16.ptr),
                        GENERIC_READ | (mode == SOWR_FIO_WRITE_APPEND ? FILE_APPEND_DATA : GENERIC_WRITE),
                        0, NULL,
                        mode == SOWR_FIO_WRITE_TRUNCATE ? TRUNCATE_EXISTING : OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);
    sowr_Vector_DestroyS(&utf16);
    return hdl;
#else
    return open(path, O_RDWR | (mode == SOWR_FIO_WRITE_APPEND ? O_APPEND :
                              (mode == SOWR_FIO_WRITE_TRUNCATE ? O_TRUNC : O_RDWR)));
#endif
}

sowr_File
sowr_File_OpenOrCreate( const char *path, sowr_FileWriteMode mode )
{
    sowr_File file = sowr_File_OpenW(path, mode);
    if (file != SOWR_INVALID_FILE_DESCRIPTOR)
        return file;
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
#endif

    char *last_dir = strrchr(path, '/');
    if (!last_dir)
    {
    // If path doesn't have the / sign, create the file directly.
#ifdef SOWR_TARGET_WINDOWS
        sowr_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
        file = CreateFileW((wchar_t *)(utf16.ptr),
                        GENERIC_READ | (mode == SOWR_FIO_WRITE_APPEND ? FILE_APPEND_DATA : GENERIC_WRITE),
                        0, NULL,
                        mode == SOWR_FIO_WRITE_TRUNCATE ? TRUNCATE_EXISTING : OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);
#else
        file = open(path, O_RDWR | O_CREAT, S_IRWXU);
#endif
    }
    else
    {
        sowr_String str = sowr_String_CreateS();
        const char *path_r = path;
        while (*path_r)
        {
            while (*path_r != '/')
            {
                sowr_String_PushC(&str, *path_r);
                path_r++;
            }
            if (strcmp(str.ptr, "."))
            {
#ifdef SOWR_TARGET_WINDOWS
                sowr_Vector_Clear(&utf16);
                sowr_Unicode_UTF8ToUTF16((unsigned char *)str.ptr, &utf16);
                CreateDirectoryW((wchar_t *)(utf16.ptr), NULL);
#else
                mkdir(str.ptr, S_IRWXU);
#endif
            }
            if (path_r == last_dir)
                break;
            sowr_String_PushC(&str, '/');
            path_r++;
        }
#ifdef SOWR_TARGET_WINDOWS
        sowr_Vector_Clear(&utf16);
        sowr_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
        file = CreateFileW((wchar_t *)(utf16.ptr), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
#else
        file = open(path, O_RDWR | O_CREAT, S_IRWXU);
#endif
        sowr_String_DestroyS(&str);
    }
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector_DestroyS(&utf16);
#endif
    return file;
}

void
sowr_File_Close( sowr_File file )
{
    if (file != SOWR_INVALID_FILE_DESCRIPTOR)
#ifdef SOWR_TARGET_WINDOWS
        CloseHandle(file);
#else
        close(file);
#endif
}

void
sowr_File_Mkdir( const char *path )
{
    sowr_String str = sowr_String_CreateS();
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
#endif
    while (*path)
    {
        while (*path != '/')
        {
            sowr_String_PushC(&str, *path);
            path++;
        }
#ifdef SOWR_TARGET_WINDOWS
        sowr_Vector_Clear(&utf16);
        sowr_Unicode_UTF8ToUTF16((unsigned char *)str.ptr, &utf16);
        CreateDirectoryW((wchar_t *)(utf16.ptr), NULL);
#else
        mkdir(str.ptr, S_IRWXU);
#endif
        sowr_String_PushC(&str, '/');
        path++;
    }
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector_DestroyS(&utf16);
#endif
    sowr_String_DestroyS(&str);
}

void
sowr_File_WalkDir( const char *path, sowr_DirWalkFunc func )
{
    sowr_String str = sowr_String_FromS(path);
#ifdef SOWR_TARGET_WINDOWS
    sowr_String_PushS(&str, "/*");
    sowr_Vector utf8 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Vector utf16 = sowr_Vector_CreateS(sizeof(unsigned char), NULL);
    sowr_Unicode_UTF8ToUTF16((unsigned char *)str.ptr, &utf16);

    WIN32_FIND_DATAW find_data;
    HANDLE f_entry = FindFirstFileW(utf16.ptr, &find_data);
    if (f_entry == INVALID_HANDLE_VALUE)
    {
        sowr_String_DestroyS(&str);
        sowr_Vector_DestroyS(&utf8);
        sowr_Vector_DestroyS(&utf16);
        return;
    }

    do
    {
        sowr_Unicode_UTF16ToUTF8((unsigned char *)find_data.cFileName, &utf8);
        if (!strcmp(utf8.ptr, ".") || !strcmp(utf8.ptr, ".."))
        {
            sowr_Vector_Clear(&utf8);
            continue;
        }

        sowr_String_Clear(&str);
        sowr_String_PushS(&str, path);
        sowr_String_PushC(&str, '/');
        sowr_String_PushS(&str, utf8.ptr);
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            sowr_File_WalkDir(str.ptr, func);
        else
            func(str.ptr);

        sowr_Vector_Clear(&utf8);
    } while (FindNextFileW(f_entry, &find_data));

    FindClose(f_entry);
#else
    DIR *dir = opendir(str.ptr);
    struct dirent64 *f_entry = readdir64(dir);
    struct stat64 f_stat;

    if (!dir || !f_entry)
    {
        sowr_String_DestroyS(&str);
        return;
    }

    do
    {
        if (!strcmp(f_entry->d_name, ".") || !strcmp(f_entry->d_name, ".."))
            continue;

        sowr_String_Clear(&str);
        sowr_String_PushS(&str, path);
        sowr_String_PushC(&str, '/');
        sowr_String_PushS(&str, f_entry->d_name);
        if (!stat64(str.ptr, &f_stat) && S_ISDIR(f_stat.st_mode))
            sowr_File_WalkDir(str.ptr, func);
        else
            func(str.ptr);
    } while ((f_entry = readdir64(dir)));

    closedir(dir);
#endif
#ifdef SOWR_TARGET_WINDOWS
    sowr_Vector_DestroyS(&utf16);
    sowr_Vector_DestroyS(&utf8);
#endif
    sowr_String_DestroyS(&str);
}

size_t
sowr_File_GetSize( sowr_File file )
{
#ifdef SOWR_TARGET_WINDOWS
    LARGE_INTEGER sz;
    GetFileSizeEx(file, &sz);
    return sz.QuadPart;
#else
    struct stat64 f_stat;
    fstat64(file, &f_stat);
    return f_stat.st_size;
#endif
}

bool
sowr_File_ReadContent( sowr_File file, void *buffer, size_t sz )
{
#ifdef SOWR_TARGET_WINDOWS
    return ReadFile(file, buffer, sz, NULL, NULL);
#else
    return read(file, buffer, sz);
#endif
}

bool
sowr_File_WriteContent( sowr_File file, const void *buffer, size_t sz )
{
#ifdef SOWR_TARGET_WINDOWS
    return WriteFile(file, buffer, sz, NULL, NULL);
#else
    return write(file, buffer, sz);
#endif
}

void
sowr_File_WriteContents( sowr_File file, size_t count, ... )
{
    va_list args;
    va_start(args, count);
    sowr_File_WriteContentsV(file, count, &args);
    va_end(args);
}

void
sowr_File_WriteContentsV( sowr_File file, size_t count, va_list *args )
{
    va_list args_cpy;
    va_copy(args_cpy, *args);
    for (size_t i = 0ULL; i < count; i++)
    {
        sowr_GenericType data = va_arg(args_cpy, sowr_GenericType);
        switch (data.type_name)
        {
            case SOWR_TYPE_BOOL:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_bool), sizeof(bool));
                break;
            }
            case SOWR_TYPE_CHAR:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_char), sizeof(char));
                break;
            }
            case SOWR_TYPE_UCHAR:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uchar), sizeof(unsigned char));
                break;
            }
            case SOWR_TYPE_SHORT:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_short), sizeof(short));
                break;
            }
            case SOWR_TYPE_USHORT:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_ushort), sizeof(unsigned short));
                break;
            }
            case SOWR_TYPE_INT:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int), sizeof(int));
                break;
            }
            case SOWR_TYPE_UINT:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint), sizeof(unsigned int));
                break;
            }
            case SOWR_TYPE_LONG:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_long), sizeof(long));
                break;
            }
            case SOWR_TYPE_ULONG:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_ulong), sizeof(unsigned long));
                break;
            }
            case SOWR_TYPE_LONGLONG:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_long_long), sizeof(long long));
                break;
            }
            case SOWR_TYPE_ULONGLONG:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_ulong_long), sizeof(unsigned long long));
                break;
            }
            case SOWR_TYPE_CHAR16:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_char16), sizeof(char16_t));
                break;
            }
            case SOWR_TYPE_CHAR32:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_char32), sizeof(char32_t));
                break;
            }
            case SOWR_TYPE_INT_8:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_8), sizeof(int8_t));
                break;
            }
            case SOWR_TYPE_UINT_8:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_8), sizeof(uint8_t));
                break;
            }
            case SOWR_TYPE_INT_16:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_16), sizeof(int16_t));
                break;
            }
            case SOWR_TYPE_UINT_16:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_16), sizeof(uint16_t));
                break;
            }
            case SOWR_TYPE_INT_32:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_32), sizeof(int32_t));
                break;
            }
            case SOWR_TYPE_UINT_32:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_32), sizeof(uint32_t));
                break;
            }
            case SOWR_TYPE_INT_64:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_64), sizeof(int64_t));
                break;
            }
            case SOWR_TYPE_UINT_64:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_64), sizeof(uint64_t));
                break;
            }
            case SOWR_TYPE_INT_LEAST_8:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_least8), sizeof(int_least8_t));
                break;
            }
            case SOWR_TYPE_UINT_LEAST_8:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_least8), sizeof(uint_least8_t));
                break;
            }
            case SOWR_TYPE_INT_LEAST_16:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_least16), sizeof(int_least16_t));
                break;
            }
            case SOWR_TYPE_UINT_LEAST_16:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_least16), sizeof(uint_least16_t));
                break;
            }
            case SOWR_TYPE_INT_LEAST_32:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_least32), sizeof(int_least32_t));
                break;
            }
            case SOWR_TYPE_UINT_LEAST_32:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_least32), sizeof(uint_least32_t));
                break;
            }
            case SOWR_TYPE_INT_LEAST_64:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_least64), sizeof(int_least64_t));
                break;
            }
            case SOWR_TYPE_UINT_LEAST_64:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_least64), sizeof(uint_least64_t));
                break;
            }
            case SOWR_TYPE_INT_FAST_8:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_fast8), sizeof(int_fast8_t));
                break;
            }
            case SOWR_TYPE_UINT_FAST_8:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_fast8), sizeof(uint_fast8_t));
                break;
            }
            case SOWR_TYPE_INT_FAST_16:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_fast16), sizeof(int_fast16_t));
                break;
            }
            case SOWR_TYPE_UINT_FAST_16:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_fast16), sizeof(uint_fast16_t));
                break;
            }
            case SOWR_TYPE_INT_FAST_32:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_fast32), sizeof(int_fast32_t));
                break;
            }
            case SOWR_TYPE_UINT_FAST_32:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_fast32), sizeof(uint_fast32_t));
                break;
            }
            case SOWR_TYPE_INT_FAST_64:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_fast64), sizeof(int_fast64_t));
                break;
            }
            case SOWR_TYPE_UINT_FAST_64:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_fast64), sizeof(uint_fast64_t));
                break;
            }
            case SOWR_TYPE_SIZE:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_size), sizeof(size_t));
                break;
            }
            case SOWR_TYPE_PTR_DIFF:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_ptr_diff), sizeof(ptrdiff_t));
                break;
            }
            case SOWR_TYPE_INT_MAX:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_int_max), sizeof(intmax_t));
                break;
            }
            case SOWR_TYPE_UINT_MAX:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_uint_max), sizeof(uintmax_t));
                break;
            }

            case SOWR_TYPE_FLOAT:
            {
                sowr_File_WriteContent(file, &(data.data.flt.as_float), sizeof(float));
                break;
            }
            case SOWR_TYPE_DOUBLE:
            {
                sowr_File_WriteContent(file, &(data.data.flt.as_double), sizeof(double));
                break;
            }
            case SOWR_TYPE_LDOUBLE:
            {
                sowr_File_WriteContent(file, &(data.data.flt.as_ldouble), sizeof(long double));
                break;
            }

            case SOWR_TYPE_STRING:
            {
                const char *str = data.data.reg.as_string;
                size_t len = strlen(str);
                sowr_File_WriteContent(file, str, len);
                break;
            }
            case SOWR_TYPE_FUNC_PTR:
            case SOWR_TYPE_VOID_PTR:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_void_ptr), sizeof(void *));
                break;
            }
            case SOWR_TYPE_PTR:
            {
                sowr_File_WriteContent(file, &(data.data.reg.as_ptr), sizeof(sowr_Ptr));
                break;
            }
            case SOWR_TYPE_ARRAY:
            {
                sowr_File_WriteContent(file, data.data.reg.as_arr, data.data_size);
                break;
            }
            default:
                sowr_File_WriteContent(file, &(data.data), sizeof(sowr_GenericTypeData));
        }
    }
    va_end(args_cpy);
}
