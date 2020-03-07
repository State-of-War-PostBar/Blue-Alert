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

#ifndef _SOWR_VECTOR_H_
#define _SOWR_VECTOR_H_

#include <pch.h>

#include "../../memory/heap_memory.h"

typedef void (*sowr_VecFreeFunc_t)(void *);

#define SOWR_DEF_VECTOR_OF_TYPE(type_name)                                                       \
typedef struct sowr_Vector_##type_name##_t                                                       \
{                                                                                                \
    size_t length;                                                                               \
    size_t capacity;                                                                             \
    size_t elem_size;                                                                            \
    type_name *ptr;                                                                              \
    sowr_VecFreeFunc_t free_func;                                                                \
    unsigned char __align__16__[(3 * sizeof(size_t) +                                            \
                                 sizeof(type_name *) +                                           \
                                 sizeof(sowr_VecFreeFunc_t)) % 16];                              \
} sowr_Vector_##type_name

#define SOWR_VECTOR_INIT(type_name, var_name, free_func_)                                        \
sowr_Vector_##type_name var_name =                                                               \
{                                                                                                \
    .length = 0,                                                                                 \
    .capacity = 0,                                                                               \
    .elem_size = sizeof(type_name),                                                              \
    .ptr = NULL,                                                                                 \
    .free_func = free_func_                                                                      \
}

#define SOWR_VECTOR_EXPAND(v)                                                                    \
{                                                                                                \
    if (!v.capacity)                                                                             \
    {                                                                                            \
        v.capacity = 1;                                                                          \
        v.ptr = sowr_HeapAlloc(v.elem_size * v.capacity);                                        \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        v.capacity *= 2;                                                                         \
        v.ptr = sowr_ReAlloc(v.ptr, v.elem_size * v.capacity);                                   \
    }                                                                                            \
}

#define SOWR_VECTOR_EXPAND_UNTIL(v, i)                                                           \
    while (v.capacity < i) SOWR_VECTOR_EXPAND(v);

#define SOWR_VECTOR_WALK(v, f)                                                                   \
{                                                                                                \
    for (char *ptr = (char *)v.ptr; ptr != (char *)SOWR_VECTOR_BACK(v); ptr += v.elem_size)      \
        f((void *)ptr);                                                                          \
}

#define SOWR_VECTOR_GET(v, i)    &(v.ptr[i])
#define SOWR_VECTOR_BACK(v)      &(v.ptr[v.length])

#define SOWR_VECTOR_CLEAR(v)                                                                     \
{                                                                                                \
    if (v.free_func)                                                                             \
        SOWR_VECTOR_WALK(v, v.free_func);                                                        \
    v.length = 0;                                                                                \
}

#define SOWR_VECTOR_INSERT(v, ptr_element, i)                                                    \
{                                                                                                \
    if (i >= v.length)                                                                           \
    {                                                                                            \
        SOWR_VECTOR_PUSH(v, ptr_element);                                                        \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        SOWR_VECTOR_EXPAND_UNTIL(v, v.length + 1);                                               \
        void *ptr_inserting = &(v.ptr[i]);                                                       \
        void *ptr_shifting = &(v.ptr[i + 1]);                                                    \
        size_t bytes_to_shift = v.elem_size * (v.length - i);                                    \
        memmove(ptr_shifting, ptr_inserting, bytes_to_shift);                                    \
        memmove(ptr_inserting, ptr_element, v.elem_size);                                        \
        v.length++;                                                                              \
    }                                                                                            \
}

#define SOWR_VECTOR_DELETE(v, i)                                                                 \
{                                                                                                \
    if (v.free_func)                                                                             \
    {                                                                                            \
        v.free_func((void *)&(v.ptr[i]));                                                        \
    }                                                                                            \
    void *ptr_shifting = &(v.ptr[i]);                                                            \
    void *ptr_data = &(v.ptr[i + 1]);                                                            \
    size_t bytes_to_shift = v.elem_size * (v.length - i);                                        \
    memmove(ptr_shifting, ptr_data, bytes_to_shift);                                             \
    v.length--;                                                                                  \
}

#define SOWR_VECTOR_PUSH(v, ptr_element)                                                         \
{                                                                                                \
    SOWR_VECTOR_EXPAND_UNTIL(v, v.length + 1);                                                   \
    void *ptr = &(v.ptr[v.length]);                                                              \
    memmove(ptr, ptr_element, v.elem_size);                                                      \
    v.length++;                                                                                  \
}

#define SOWR_VECTOR_POP(v, ptr_retrieve)                                                         \
{                                                                                                \
    if (ptr_retrieve)                                                                            \
        memmove(ptr_retrieve, &(v.ptr[v.length - 1]), v.elem_size);                              \
    v.length--;                                                                                  \
}

#define SOWR_VECTOR_SHRINK_TO_FIT(v)                                                             \
{                                                                                                \
    if (v.capacity > v.length)                                                                   \
    {                                                                                            \
        v.ptr = sowr_ReAlloc(v.ptr, v.length * v.elem_size);                                     \
        v.capacity = v.length;                                                                   \
    }                                                                                            \
}

#define SOWR_VECTOR_DESTROY(v)                                                                   \
{                                                                                                \
    if (v.free_func)                                                                             \
    {                                                                                            \
        SOWR_VECTOR_WALK(v, v.free_func);                                                        \
    }                                                                                            \
    sowr_HeapFree(v.ptr);                                                                        \
}

#endif
