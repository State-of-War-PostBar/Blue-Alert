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

#ifndef SOWR_LIB_CONTAINER_VECTOR_H
#define SOWR_LIB_CONTAINER_VECTOR_H

#include <pch.h>

#include "../memory/heap_memory.h"

typedef void (*sowr_VecFreeFunc)(void *);

#define SOWR_DEF_VECTOR_OF_TYPE(type_name)                                                       \
typedef struct                                                                                   \
{                                                                                                \
    size_t length;                                                                               \
    size_t capacity;                                                                             \
    size_t elem_size;                                                                            \
    type_name *ptr;                                                                              \
    sowr_VecFreeFunc free_func;                                                                  \
} sowr_Vector_##type_name;

#define SOWR_VECTOR_INIT(type_name, pv, free_func_)                                              \
{                                                                                                \
    pv = sowr_HeapAlloc(sizeof(sowr_CVector_##type_name));                                       \
    pv->length = 0;                                                                              \
    pv->capacity = 0;                                                                            \
    pv->elem_size = sizeof(type_name);                                                           \
    pv->ptr = NULL;                                                                              \
    pv->free_func = free_func_;                                                                  \
}

#define SOWR_VECTOR_EXPAND(pv)                                                                   \
{                                                                                                \
    if (!pv->capacity)                                                                           \
    {                                                                                            \
        pv->capacity = 1;                                                                        \
        pv->ptr = sowr_HeapAlloc(pv->elem_size * pv->capacity);                                  \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        pv->capacity *= 2;                                                                       \
        pv->ptr = sowr_ReAlloc(pv->elem_size * pv->capacity, pv->ptr);                           \
    }                                                                                            \
}

#define SOWR_VECTOR_EXPAND_UNTIL(pv, i)                                                          \
{                                                                                                \
    while (pv->capacity < i)                                                                     \
        SOWR_VECTOR_EXPAND(pv);                                                                  \
}

#define SOWR_VECTOR_WALK(pv, f)                                                                  \
{                                                                                                \
    for (char *ptr = (char *)pv->ptr; ptr != (char *)SOWR_VECTOR_BACK(pv); ptr += pv->elem_size) \
        f((void *)ptr);                                                                          \
}

#define SOWR_VECTOR_GET(pv, i)    &(pv->ptr[i])
#define SOWR_VECTOR_FRONT(pv)     pv->ptr
#define SOWR_VECTOR_BACK(pv)      &(pv->ptr[pv->length])

#define SOWR_VECTOR_CLEAR(pv)                                                                    \
{                                                                                                \
    if (pv->free_func)                                                                           \
    {                                                                                            \
        SOWR_VECTOR_WALK(pv, pv->free_func);                                                     \
    }                                                                                            \
    pv->length = 0;                                                                              \
}

#define SOWR_VECTOR_INSERT(pv, ptr_element, i)                                                   \
{                                                                                                \
    if (i >= pv->length)                                                                         \
    {                                                                                            \
        SOWR_VECTOR_PUSH(pv, ptr_element);                                                       \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        SOWR_VECTOR_EXPAND_UNTIL(pv, pv->length + 1);                                            \
        void *ptr_inserting = &(pv->ptr[i]);                                                     \
        void *ptr_shifting = &(pv->ptr[i + 1]);                                                  \
        size_t bytes_to_shift = pv->elem_size * (pv->length - i);                                \
        memmove(ptr_shifting, ptr_inserting, bytes_to_shift);                                    \
        memmove(ptr_inserting, ptr_element, pv->elem_size);                                      \
        pv->length++;                                                                            \
    }                                                                                            \
}

#define SOWR_VECTOR_INSERT_CPY(pv, ptr_element, i)                                               \
{                                                                                                \
    if (i >= pv->length)                                                                         \
    {                                                                                            \
        SOWR_VECTOR_PUSH_CPY(pv, ptr_element);                                                   \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        SOWR_VECTOR_EXPAND_UNTIL(pv, pv->length + 1);                                            \
        void *ptr_inserting = &(pv->ptr[i]);                                                     \
        void *ptr_shifting = &(pv->ptr[i + 1]);                                                  \
        size_t bytes_to_shift = pv->elem_size * (pv->length - i);                                \
        memcpy(ptr_shifting, ptr_inserting, bytes_to_shift);                                     \
        memcpy(ptr_inserting, ptr_element, pv->elem_size);                                       \
        pv->length++;                                                                            \
    }                                                                                            \
}

#define SOWR_VECTOR_DELETE(pv, i)                                                                \
{                                                                                                \
    if (pv->free_func)                                                                           \
    {                                                                                            \
        pv->free_func((void *)&(pv->ptr[i]));                                                    \
    }                                                                                            \
    void *ptr_shifting = &(pv->ptr[i]);                                                          \
    void *ptr_data = &(pv->ptr[i + 1]);                                                          \
    size_t bytes_to_shift = pv->elem_size * (pv->length - i - 1);                                \
    memmove(ptr_shifting, ptr_data, bytes_to_shift);                                             \
    pv->length--;                                                                                \
}

#define SOWR_VECTOR_TAKE(pv, i, ptr_retrieve)                                                    \
{                                                                                                \
    if (i >= pv->length)                                                                         \
    {                                                                                            \
        SOWR_VECTOR_POP(pv, ptr_retrieve);                                                       \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        void *ptr_shifting = SOWR_VECTOR_GET(pv, i);                                             \
        void *ptr_data = SOWR_VECTOR_GET(pv, i + 1);                                             \
        size_t bytes_to_shift = pv->elem_size * (pv->length - i - 1);                            \
        memmove(ptr_retrieve, ptr_shifting, pv->elem_size);                                      \
        memmove(ptr_shifting, ptr_data, bytes_to_shift);                                         \
        pv->length--;                                                                            \
    }                                                                                            \
}

#define SOWR_VECTOR_PUSH(pv, ptr_element)                                                        \
{                                                                                                \
    SOWR_VECTOR_EXPAND_UNTIL(pv, pv->length + 1);                                                \
    void *ptr = &(pv->ptr[pv->length]);                                                          \
    memmove(ptr, ptr_element, pv->elem_size);                                                    \
    pv->length++;                                                                                \
}

#define SOWR_VECTOR_PUSH_CPY(pv, ptr_element)                                                    \
{                                                                                                \
    SOWR_VECTOR_EXPAND_UNTIL(pv, pv->length + 1);                                                \
    void *ptr = &(pv->ptr[pv->length]);                                                          \
    memcpy(ptr, ptr_element, pv->elem_size);                                                     \
    pv->length++;                                                                                \
}

#define SOWR_VECTOR_POP(pv, ptr_retrieve)                                                        \
{                                                                                                \
    if (ptr_retrieve)                                                                            \
    {                                                                                            \
        memmove(ptr_retrieve, &(pv->ptr[pv->length - 1]), pv->elem_size);                        \
    }                                                                                            \
    pv->length--;                                                                                \
}

#define SOWR_VECTOR_SHRINK_TO_FIT(pv)                                                            \
{                                                                                                \
    if (pv->capacity > pv->length && pv->length)                                                 \
    {                                                                                            \
        pv->ptr = sowr_ReAlloc(pv->length * pv->elem_size, pv->ptr);                             \
        pv->capacity = pv->length;                                                               \
    }                                                                                            \
}

#define SOWR_VECTOR_DESTROY(pv)                                                                  \
{                                                                                                \
    if (pv->free_func)                                                                           \
    {                                                                                            \
        SOWR_VECTOR_WALK(pv, pv->free_func);                                                     \
    }                                                                                            \
    sowr_HeapFree(pv->ptr);                                                                      \
    sowr_HeapFree(pv);                                                                           \
}

#endif // !SOWR_LIB_CONTAINER_VECTOR_H
