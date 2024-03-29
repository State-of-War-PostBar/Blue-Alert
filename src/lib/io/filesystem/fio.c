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

#include "fio.h"

#include "../../container/string.h"
#include "../../data/unicode.h"
#include "../../memory/heap_memory.h"
#include "../../type/generic.h"

blrt_File
blrt_File_OpenR( const char *path )
{
#ifdef BLRT_TARGET_WINDOWS
    blrt_Vector utf16 = blrt_Vector_CreateS(sizeof(unsigned char), NULL);
    blrt_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
    blrt_File hdl = CreateFileW((wchar_t *)(utf16.ptr), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    blrt_Vector_DestroyS(&utf16);
    return hdl;
#else
    return open(path, O_RDONLY);
#endif
}

blrt_File
blrt_File_OpenW( const char *path, blrt_FileWriteMode mode )
{
#ifdef BLRT_TARGET_WINDOWS
    blrt_Vector utf16 = blrt_Vector_CreateS(sizeof(unsigned char), NULL);
    blrt_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
    blrt_File hdl = CreateFileW((wchar_t *)(utf16.ptr),
                        GENERIC_READ | (mode == BLRT_FIO_WRITE_APPEND ? FILE_APPEND_DATA : GENERIC_WRITE),
                        0, NULL,
                        mode == BLRT_FIO_WRITE_TRUNCATE ? TRUNCATE_EXISTING : OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);
    blrt_Vector_DestroyS(&utf16);
    return hdl;
#else
    return open(path, O_RDWR | (mode == BLRT_FIO_WRITE_APPEND ? O_APPEND :
                              (mode == BLRT_FIO_WRITE_TRUNCATE ? O_TRUNC : O_RDWR)));
#endif
}

blrt_File
blrt_File_OpenOrCreate( const char *path, blrt_FileWriteMode mode )
{
    blrt_File file = blrt_File_OpenW(path, mode);
    if (file != BLRT_INVALID_FILE_DESCRIPTOR)
        return file;
#ifdef BLRT_TARGET_WINDOWS
    blrt_Vector utf16 = blrt_Vector_CreateS(sizeof(unsigned char), NULL);
#endif

    char *last_dir = strrchr(path, '/');
    if (!last_dir)
    {
    // If path doesn't have the / sign, create the file directly.
#ifdef BLRT_TARGET_WINDOWS
        blrt_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
        file = CreateFileW((wchar_t *)(utf16.ptr),
                        GENERIC_READ | (mode == BLRT_FIO_WRITE_APPEND ? FILE_APPEND_DATA : GENERIC_WRITE),
                        0, NULL,
                        mode == BLRT_FIO_WRITE_TRUNCATE ? TRUNCATE_EXISTING : OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL);
#else
        file = open(path, O_RDWR | O_CREAT, S_IRWXU);
#endif
    }
    else
    {
        blrt_String str = blrt_String_CreateS();
        const char *path_r = path;
        while (*path_r)
        {
            while (*path_r != '/')
            {
                blrt_String_PushC(&str, *path_r);
                path_r++;
            }
            if (strcmp(str.ptr, "."))
            {
#ifdef BLRT_TARGET_WINDOWS
                blrt_Vector_Clear(&utf16);
                blrt_Unicode_UTF8ToUTF16((unsigned char *)str.ptr, &utf16);
                CreateDirectoryW((wchar_t *)(utf16.ptr), NULL);
#else
                mkdir(str.ptr, S_IRWXU);
#endif
            }
            if (path_r == last_dir)
                break;
            blrt_String_PushC(&str, '/');
            path_r++;
        }
#ifdef BLRT_TARGET_WINDOWS
        blrt_Vector_Clear(&utf16);
        blrt_Unicode_UTF8ToUTF16((unsigned char *)path, &utf16);
        file = CreateFileW((wchar_t *)(utf16.ptr), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
#else
        file = open(path, O_RDWR | O_CREAT, S_IRWXU);
#endif
        blrt_String_DestroyS(&str);
    }
#ifdef BLRT_TARGET_WINDOWS
    blrt_Vector_DestroyS(&utf16);
#endif
    return file;
}

void
blrt_File_Close( blrt_File file )
{
    if (file != BLRT_INVALID_FILE_DESCRIPTOR)
#ifdef BLRT_TARGET_WINDOWS
        CloseHandle(file);
#else
        close(file);
#endif
}

void
blrt_File_Mkdir( const char *path )
{
    blrt_String str = blrt_String_CreateS();
#ifdef BLRT_TARGET_WINDOWS
    blrt_Vector utf16 = blrt_Vector_CreateS(sizeof(unsigned char), NULL);
#endif
    while (*path)
    {
        while (*path != '/')
        {
            blrt_String_PushC(&str, *path);
            path++;
        }
#ifdef BLRT_TARGET_WINDOWS
        blrt_Vector_Clear(&utf16);
        blrt_Unicode_UTF8ToUTF16((unsigned char *)str.ptr, &utf16);
        CreateDirectoryW((wchar_t *)(utf16.ptr), NULL);
#else
        mkdir(str.ptr, S_IRWXU);
#endif
        blrt_String_PushC(&str, '/');
        path++;
    }
#ifdef BLRT_TARGET_WINDOWS
    blrt_Vector_DestroyS(&utf16);
#endif
    blrt_String_DestroyS(&str);
}

void
blrt_File_WalkDir( const char *path, blrt_DirWalkFunc func )
{
    blrt_String str = blrt_String_FromS(path);
#ifdef BLRT_TARGET_WINDOWS
    blrt_String_PushS(&str, "/*");
    blrt_Vector utf8 = blrt_Vector_CreateS(sizeof(unsigned char), NULL);
    blrt_Vector utf16 = blrt_Vector_CreateS(sizeof(unsigned char), NULL);
    blrt_Unicode_UTF8ToUTF16((unsigned char *)str.ptr, &utf16);

    WIN32_FIND_DATAW find_data;
    HANDLE f_entry = FindFirstFileW(utf16.ptr, &find_data);
    if (f_entry == INVALID_HANDLE_VALUE)
    {
        blrt_String_DestroyS(&str);
        blrt_Vector_DestroyS(&utf8);
        blrt_Vector_DestroyS(&utf16);
        return;
    }

    do
    {
        blrt_Unicode_UTF16ToUTF8((unsigned char *)find_data.cFileName, &utf8);
        if (!strcmp(utf8.ptr, ".") || !strcmp(utf8.ptr, ".."))
        {
            blrt_Vector_Clear(&utf8);
            continue;
        }

        blrt_String_Clear(&str);
        blrt_String_PushS(&str, path);
        blrt_String_PushC(&str, '/');
        blrt_String_PushS(&str, utf8.ptr);
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            blrt_File_WalkDir(str.ptr, func);
        else
            func(str.ptr);

        blrt_Vector_Clear(&utf8);
    } while (FindNextFileW(f_entry, &find_data));

    FindClose(f_entry);
#else
    DIR *dir = opendir(str.ptr);

    if (!dir)
    {
        blrt_String_DestroyS(&str);
        return;
    }

    struct dirent *f_entry = readdir(dir);
    struct stat f_stat;

    if (!f_entry)
    {
        blrt_String_DestroyS(&str);
        return;
    }

    do
    {
        if (!strcmp(f_entry->d_name, ".") || !strcmp(f_entry->d_name, ".."))
            continue;

        blrt_String_Clear(&str);
        blrt_String_PushS(&str, path);
        blrt_String_PushC(&str, '/');
        blrt_String_PushS(&str, f_entry->d_name);
        if (!stat(str.ptr, &f_stat) && S_ISDIR(f_stat.st_mode))
            blrt_File_WalkDir(str.ptr, func);
        else
            func(str.ptr);
    } while ((f_entry = readdir(dir)));

    closedir(dir);
#endif
#ifdef BLRT_TARGET_WINDOWS
    blrt_Vector_DestroyS(&utf16);
    blrt_Vector_DestroyS(&utf8);
#endif
    blrt_String_DestroyS(&str);
}

size_t
blrt_File_GetSize( blrt_File file )
{
#ifdef BLRT_TARGET_WINDOWS
    LARGE_INTEGER sz;
    GetFileSizeEx(file, &sz);
    return sz.QuadPart;
#else
    struct stat f_stat;
    fstat(file, &f_stat);
    return f_stat.st_size;
#endif
}

bool
blrt_File_ReadContent( blrt_File file, void *buffer, size_t sz )
{
#ifdef BLRT_TARGET_WINDOWS
    return ReadFile(file, buffer, sz, NULL, NULL);
#else
    return read(file, buffer, sz);
#endif
}

bool
blrt_File_WriteContent( blrt_File file, const void *buffer, size_t sz )
{
#ifdef BLRT_TARGET_WINDOWS
    return WriteFile(file, buffer, sz, NULL, NULL);
#else
    return write(file, buffer, sz);
#endif
}

void
blrt_File_WriteContents( blrt_File file, size_t count, ... )
{
    va_list args;
    va_start(args, count);
    blrt_File_WriteContentsV(file, count, &args);
    va_end(args);
}

void
blrt_File_WriteContentsV( blrt_File file, size_t count, va_list *args )
{
    va_list args_cpy;
    va_copy(args_cpy, *args);
    for (size_t i = 0ULL; i < count; i++)
    {
        blrt_GenericType data = va_arg(args_cpy, blrt_GenericType);
        switch (data.type)
        {
            case BLRT_TYPE_BOOL:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_bool), sizeof(bool));
                break;
            }
            case BLRT_TYPE_CHAR:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_char), sizeof(char));
                break;
            }
            case BLRT_TYPE_UCHAR:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uchar), sizeof(unsigned char));
                break;
            }
            case BLRT_TYPE_SHORT:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_short), sizeof(short));
                break;
            }
            case BLRT_TYPE_USHORT:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_ushort), sizeof(unsigned short));
                break;
            }
            case BLRT_TYPE_INT:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int), sizeof(int));
                break;
            }
            case BLRT_TYPE_UINT:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint), sizeof(unsigned int));
                break;
            }
            case BLRT_TYPE_LONG:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_long), sizeof(long));
                break;
            }
            case BLRT_TYPE_ULONG:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_ulong), sizeof(unsigned long));
                break;
            }
            case BLRT_TYPE_LONGLONG:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_long_long), sizeof(long long));
                break;
            }
            case BLRT_TYPE_ULONGLONG:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_ulong_long), sizeof(unsigned long long));
                break;
            }
            case BLRT_TYPE_CHAR16:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_char16), sizeof(char16_t));
                break;
            }
            case BLRT_TYPE_CHAR32:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_char32), sizeof(char32_t));
                break;
            }
            case BLRT_TYPE_INT_8:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_8), sizeof(int8_t));
                break;
            }
            case BLRT_TYPE_UINT_8:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_8), sizeof(uint8_t));
                break;
            }
            case BLRT_TYPE_INT_16:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_16), sizeof(int16_t));
                break;
            }
            case BLRT_TYPE_UINT_16:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_16), sizeof(uint16_t));
                break;
            }
            case BLRT_TYPE_INT_32:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_32), sizeof(int32_t));
                break;
            }
            case BLRT_TYPE_UINT_32:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_32), sizeof(uint32_t));
                break;
            }
            case BLRT_TYPE_INT_64:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_64), sizeof(int64_t));
                break;
            }
            case BLRT_TYPE_UINT_64:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_64), sizeof(uint64_t));
                break;
            }
            case BLRT_TYPE_INT_LEAST_8:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_least8), sizeof(int_least8_t));
                break;
            }
            case BLRT_TYPE_UINT_LEAST_8:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_least8), sizeof(uint_least8_t));
                break;
            }
            case BLRT_TYPE_INT_LEAST_16:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_least16), sizeof(int_least16_t));
                break;
            }
            case BLRT_TYPE_UINT_LEAST_16:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_least16), sizeof(uint_least16_t));
                break;
            }
            case BLRT_TYPE_INT_LEAST_32:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_least32), sizeof(int_least32_t));
                break;
            }
            case BLRT_TYPE_UINT_LEAST_32:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_least32), sizeof(uint_least32_t));
                break;
            }
            case BLRT_TYPE_INT_LEAST_64:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_least64), sizeof(int_least64_t));
                break;
            }
            case BLRT_TYPE_UINT_LEAST_64:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_least64), sizeof(uint_least64_t));
                break;
            }
            case BLRT_TYPE_INT_FAST_8:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_fast8), sizeof(int_fast8_t));
                break;
            }
            case BLRT_TYPE_UINT_FAST_8:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_fast8), sizeof(uint_fast8_t));
                break;
            }
            case BLRT_TYPE_INT_FAST_16:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_fast16), sizeof(int_fast16_t));
                break;
            }
            case BLRT_TYPE_UINT_FAST_16:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_fast16), sizeof(uint_fast16_t));
                break;
            }
            case BLRT_TYPE_INT_FAST_32:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_fast32), sizeof(int_fast32_t));
                break;
            }
            case BLRT_TYPE_UINT_FAST_32:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_fast32), sizeof(uint_fast32_t));
                break;
            }
            case BLRT_TYPE_INT_FAST_64:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_fast64), sizeof(int_fast64_t));
                break;
            }
            case BLRT_TYPE_UINT_FAST_64:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_fast64), sizeof(uint_fast64_t));
                break;
            }
            case BLRT_TYPE_SIZE:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_size), sizeof(size_t));
                break;
            }
            case BLRT_TYPE_PTR_DIFF:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_ptr_diff), sizeof(ptrdiff_t));
                break;
            }
            case BLRT_TYPE_INT_MAX:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_int_max), sizeof(intmax_t));
                break;
            }
            case BLRT_TYPE_UINT_MAX:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_uint_max), sizeof(uintmax_t));
                break;
            }

            case BLRT_TYPE_FLOAT:
            {
                blrt_File_WriteContent(file, &(data.data.flt.as_float), sizeof(float));
                break;
            }
            case BLRT_TYPE_DOUBLE:
            {
                blrt_File_WriteContent(file, &(data.data.flt.as_double), sizeof(double));
                break;
            }
            case BLRT_TYPE_LDOUBLE:
            {
                blrt_File_WriteContent(file, &(data.data.flt.as_ldouble), sizeof(long double));
                break;
            }

            case BLRT_TYPE_STRING:
            {
                const char *str = data.data.reg.as_string;
                size_t len = strlen(str);
                blrt_File_WriteContent(file, str, len);
                break;
            }
            case BLRT_TYPE_FUNC_PTR:
            case BLRT_TYPE_VOID_PTR:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_void_ptr), sizeof(void *));
                break;
            }
            case BLRT_TYPE_PTR:
            {
                blrt_File_WriteContent(file, &(data.data.reg.as_ptr), sizeof(blrt_Ptr));
                break;
            }
            case BLRT_TYPE_ARRAY:
            {
                blrt_File_WriteContent(file, data.data.reg.as_arr, data.data_size);
                break;
            }
            default:
                blrt_File_WriteContent(file, &(data.data), sizeof(blrt_GenericTypeData));
        }
    }
    va_end(args_cpy);
}
