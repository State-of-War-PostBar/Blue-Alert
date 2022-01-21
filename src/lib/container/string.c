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

#include "string.h"

#include "../memory/heap_memory.h"

blrt_String *
blrt_String_Create( void )
{
    return blrt_HeapZeroAlloc(sizeof(blrt_String));
}

blrt_String
blrt_String_CreateS( void )
{
    blrt_String str =
    {
        .capacity = 0ULL,
        .length = 0ULL,
        .ptr = NULL
    };
    return str;
}

blrt_String *
blrt_String_From( const char *original )
{
    blrt_String *str = blrt_HeapAlloc(sizeof(blrt_String));
    str->length = str->capacity = strlen(original);
    str->capacity++;
    str->ptr = strdup(original);
    return str;
}

blrt_String
blrt_String_FromS( const char *original )
{
    blrt_String str;
    str.length = str.capacity = strlen(original);
    str.capacity++;
    str.ptr = strdup(original);
    return str;
}

void
blrt_String_Expand( blrt_String *str )
{
    if (!str->capacity)
    {
        str->capacity = 2ULL;
        str->ptr = blrt_HeapAlloc(sizeof(char) * 2ULL);
    }
    else
    {
        str->capacity *= 2ULL;
        str->ptr = blrt_ReAlloc(str->capacity, str->ptr);
    }
}

inline
void
blrt_String_ExpandUntil( blrt_String *str, size_t new_size )
{
    while (str->capacity < new_size)
        blrt_String_Expand(str);
}

void
blrt_String_ExpandUntilOnce( blrt_String *str, size_t new_size )
{
    if (!str->capacity)
    {
        str->capacity = new_size;
        str->ptr = blrt_HeapAlloc(sizeof(char) * new_size);
    }
    else if (str->capacity < new_size)
    {
        str->capacity = new_size;
        str->ptr = blrt_ReAlloc(new_size, str->ptr);
    }
}

void
blrt_String_ShrinkToFit( blrt_String *str )
{
    if (str->capacity > str->length && str->length)
    {
        str->ptr = blrt_ReAlloc(sizeof(char) * (str->length + 1ULL), str->ptr);
        str->capacity = str->length + 1ULL;
    }
    else if (str->capacity && !str->length)
    {
        blrt_HeapFree(str->ptr);
        str->capacity = 0ULL;
    }
}

inline
char *
blrt_String_First( const blrt_String *str )
{
    return str->ptr;
}

inline
char *
blrt_String_Last( const blrt_String *str )
{
    if (!str->length)
        return NULL;

    return str->ptr + str->length - 1ULL;
}

void
blrt_String_PushC( blrt_String *str, char data )
{
    blrt_String_ExpandUntil(str, sizeof(char) * (str->length + 2ULL));
    str->ptr[str->length] = data;
    str->ptr[str->length + 1ULL] = '\0';
    str->length++;
}

void
blrt_String_PushS( blrt_String *str, const char *data )
{
    size_t target_len = strlen(data);
    blrt_String_ExpandUntil(str, sizeof(char) * (str->length + target_len + 1ULL));
    memcpy(str->length ? str->ptr + str->length : str->ptr, data, sizeof(char) * (target_len + 1ULL));
    str->length += target_len;
}

void
blrt_String_PushFrontC( blrt_String *str, char data )
{
    blrt_String_ExpandUntil(str, sizeof(char) * (str->length + 2ULL));
    if (str->length)
    {
        memmove(str->ptr + 1ULL, str->ptr, sizeof(char) * (str->length + 1ULL));
        str->ptr[0] = data;
    }
    else
    {
        str->ptr[0] = data;
        str->ptr[1] = '\0';
    }
    str->length++;
}

void
blrt_String_PushFrontS( blrt_String *str, const char *data )
{
    if (!str->length)
    {
        blrt_String_PushS(str, data);
        return;
    }
    size_t target_len = strlen(data);
    blrt_String_ExpandUntil(str, sizeof(char) * (str->length + target_len + 1ULL));
    memmove(str->ptr + target_len, str->ptr, sizeof(char) * (str->length + 1ULL));
    memcpy(str->ptr, data, sizeof(char) * target_len);
    str->length += target_len;
}

void
blrt_String_InsertC( blrt_String *str, size_t index, char data )
{
    if (index >= str->length)
        blrt_String_PushC(str, data);
    else
    {
        blrt_String_ExpandUntil(str, sizeof(char) * (str->length + 2ULL));
        memmove(str->ptr + index + 1ULL, str->ptr + index, sizeof(char) * (str->length - index + 1ULL));
        str->ptr[index] = data;
        str->length++;
    }
}

void
blrt_String_InsertS( blrt_String *str, size_t index, const char *data )
{
    if (index >= str->length)
        blrt_String_PushS(str, data);
    else
    {
        size_t target_len = strlen(data);
        blrt_String_ExpandUntil(str, sizeof(char) * (str->length + target_len + 1UL));
        memmove(str->ptr + index + target_len, str->ptr + index, sizeof(char) * (str->length - index + 1ULL));
        memcpy(str->ptr + index, data, sizeof(char) * target_len);
        str->length += target_len;
    }
}

inline
void
blrt_String_Pop( blrt_String *str )
{
    blrt_String_PopN(str, 1ULL);
}

void
blrt_String_PopN( blrt_String *str, size_t num )
{
    if (!str->length || !num)
        return;

    if (str->length <= num)
        blrt_String_Clear(str);
    else
    {
        str->ptr[str->length - num] = '\0';
        str->length -= num;
    }
}

void
blrt_String_Sub( blrt_String *str, size_t num )
{
    if (!str->length || !num)
        return;

    if (str->length <= num)
        blrt_String_Clear(str);
    else
    {
        memmove(str->ptr, str->ptr + sizeof(char) * num, sizeof(char) * (str->length - num + 1ULL));
        str->length -= num;
    }
}

void
blrt_String_Res( blrt_String *str, size_t num )
{
    if (!str->length || !num)
        return;

    if (str->length <= num)
        return;
    else
    {
        str->ptr[num] = '\0';
        str->length = num;
    }
}

void
blrt_String_Clear( blrt_String *str )
{
    if (str->length)
    {
        str->ptr[0ULL] = '\0';
        str->length = 0ULL;
    }
}

void
blrt_String_Destroy( blrt_String *str )
{
    if (str->capacity)
        blrt_HeapFree(str->ptr);
    blrt_HeapFree(str);
}

void
blrt_String_DestroyS( blrt_String *str )
{
    if (str->capacity)
        blrt_HeapFree(str->ptr);
}
