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

#ifndef SOWR_LIB_CONTAINER_CONCURRENT_VECTOR_H
#define SOWR_LIB_CONTAINER_CONCURRENT_VECTOR_H

#include <pch.h>

#include "../memory/heap_memory.h"
#include "../../sync/multithread.h"

typedef void (*sowr_CVecFreeFunc)(void *);

#define SOWR_DEF_CVECTOR_OF_TYPE(type_name, name)                                                \
typedef struct                                                                                   \
{                                                                                                \
    size_t length;                                                                               \
    size_t capacity;                                                                             \
    size_t elem_size;                                                                            \
    type_name *ptr;                                                                              \
    sowr_CVecFreeFunc free_func;                                                                 \
    sowr_CriticalSection *mtx;                                                                   \
    atomic_bool usable;                                                                          \
    atomic_bool locked;                                                                          \
} name;

#define SOWR_CVECTOR_INIT(type_name, name, pv, free_func_)                                       \
{                                                                                                \
    pv = sowr_HeapAlloc(sizeof(name));                                                           \
    pv->length = 0;                                                                              \
    pv->capacity = 0;                                                                            \
    pv->elem_size = sizeof(type_name);                                                           \
    pv->ptr = NULL;                                                                              \
    pv->free_func = free_func_;                                                                  \
    sowr_CriticalSection *mtx = sowr_HeapAlloc(sizeof(sowr_CriticalSection));                    \
    sowr_InitCriticalSection(mtx);                                                               \
    pv->mtx = mtx;                                                                               \
    atomic_store(&pv->usable, true);                                                             \
    atomic_store(&pv->locked, false);                                                            \
}

#define SOWR_CVECTOR_EXPAND(pv)                                                                  \
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

#define SOWR_CVECTOR_EXPAND_UNTIL(pv, i)                                                         \
{                                                                                                \
    while (pv->capacity < i)                                                                     \
        SOWR_CVECTOR_EXPAND(pv);                                                                 \
}

#define SOWR_CVECTOR_WALK(pv, f)                                                                 \
{                                                                                                \
    for (char *ptr = (char *)pv->ptr; ptr != (char *)SOWR_CVECTOR_BACK(pv); ptr += pv->elem_size)\
        f((void *)ptr);                                                                          \
}

#define SOWR_CVECTOR_LOCK(pv)                                                                    \
{                                                                                                \
    if (!atomic_load(&pv->locked))                                                               \
    {                                                                                            \
        sowr_EnterCriticalSection(pv->mtx);                                                      \
        atomic_store(&pv->locked, true);                                                         \
    }                                                                                            \
}

#define SOWR_CVECTOR_UNLOCK(pv)                                                                  \
{                                                                                                \
    if (atomic_load(&pv->locked))                                                                \
    {                                                                                            \
        atomic_store(&pv->locked, false);                                                        \
        sowr_LeaveCriticalSection(pv->mtx);                                                      \
    }                                                                                            \
}

#define SOWR_CVECTOR_GET(pv, i)    &(pv->ptr[i])
#define SOWR_CVECTOR_FRONT(pv)     pv->ptr
#define SOWR_CVECTOR_BACK(pv)      &(pv->ptr[pv->length])

#define SOWR_CVECTOR_WALK_S(pv, f)                                                               \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        SOWR_CVECTOR_LOCK(pv);                                                                   \
        SOWR_CVECTOR_WALK(pv, f);                                                                \
        SOWR_CVECTOR_UNLOCK(pv);                                                                 \
    }                                                                                            \
}

#define SOWR_CVECTOR_CLEAR(pv)                                                                   \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        SOWR_CVECTOR_LOCK(pv);                                                                   \
        if (pv->free_func)                                                                       \
        {                                                                                        \
            SOWR_CVECTOR_WALK(pv, pv->free_func);                                                \
        }                                                                                        \
        pv->length = 0;                                                                          \
        SOWR_CVECTOR_UNLOCK(pv);                                                                 \
    }                                                                                            \
}

#define SOWR_CVECTOR_INSERT(pv, ptr_element, i)                                                  \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        if (i >= pv->length)                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_PUSH(pv, ptr_element);                                                  \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_LOCK(pv);                                                               \
            SOWR_CVECTOR_EXPAND_UNTIL(pv, pv->length + 1);                                       \
            void *ptr_inserting = &(pv->ptr[i]);                                                 \
            void *ptr_shifting = &(pv->ptr[i + 1]);                                              \
            size_t bytes_to_shift = pv->elem_size * (pv->length - i);                            \
            memmove(ptr_shifting, ptr_inserting, bytes_to_shift);                                \
            memmove(ptr_inserting, ptr_element, pv->elem_size);                                  \
            pv->length++;                                                                        \
            SOWR_CVECTOR_UNLOCK(pv);                                                             \
        }                                                                                        \
    }                                                                                            \
}

#define SOWR_CVECTOR_INSERT_CPY(pv, ptr_element, i)                                              \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        if (i >= pv->length)                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_PUSH_CPY(pv, ptr_element);                                              \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_LOCK(pv);                                                               \
            SOWR_CVECTOR_EXPAND_UNTIL(pv, pv->length + 1);                                       \
            void *ptr_inserting = &(pv->ptr[i]);                                                 \
            void *ptr_shifting = &(pv->ptr[i + 1]);                                              \
            size_t bytes_to_shift = pv->elem_size * (pv->length - i);                            \
            memcpy(ptr_shifting, ptr_inserting, bytes_to_shift);                                 \
            memcpy(ptr_inserting, ptr_element, pv->elem_size);                                   \
            pv->length++;                                                                        \
            SOWR_CVECTOR_UNLOCK(pv);                                                             \
        }                                                                                        \
    }                                                                                            \
}

#define SOWR_CVECTOR_REPLACE(pv, ptr_element, i)                                                 \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        if (i >= pv->length)                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_PUSH(pv, ptr_element);                                                  \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_LOCK(pv);                                                               \
            if (pv->free_func)                                                                   \
            {                                                                                    \
                pv->free_func((void *)&(pv->ptr[i]));                                            \
            }                                                                                    \
            memmove(&(pv->ptr[i]), ptr_element, pv->elem_size);                                  \
            SOWR_CVECTOR_UNLOCK(pv);                                                             \
        }                                                                                        \
    }                                                                                            \
}

#define SOWR_CVECTOR_REPLACE_CPY(pv, ptr_element, i)                                             \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        if (i >= pv->length)                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_PUSH_CPY(pv, ptr_element);                                              \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_LOCK(pv);                                                               \
            if (pv->free_func)                                                                   \
            {                                                                                    \
                pv->free_func((void *)&(pv->ptr[i]));                                            \
            }                                                                                    \
            memcpy(&(pv->ptr[i]), ptr_element, pv->elem_size);                                   \
            SOWR_CVECTOR_UNLOCK(pv);                                                             \
        }                                                                                        \
    }                                                                                            \
}

#define SOWR_CVECTOR_DELETE(pv, i)                                                               \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        SOWR_CVECTOR_LOCK(pv);                                                                   \
        if (pv->free_func)                                                                       \
        {                                                                                        \
            pv->free_func((void *)&(pv->ptr[i]));                                                \
        }                                                                                        \
        void *ptr_shifting = &(pv->ptr[i]);                                                      \
        void *ptr_data = &(pv->ptr[i + 1]);                                                      \
        size_t bytes_to_shift = pv->elem_size * (pv->length - i - 1);                            \
        memmove(ptr_shifting, ptr_data, bytes_to_shift);                                         \
        pv->length--;                                                                            \
        SOWR_CVECTOR_UNLOCK(pv);                                                                 \
    }                                                                                            \
}

#define SOWR_CVECTOR_TAKE(pv, i, ptr_retrieve)                                                   \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        if (i >= pv->length)                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_POP(pv, ptr_retrieve);                                                  \
        }                                                                                        \
        else                                                                                     \
        {                                                                                        \
            SOWR_CVECTOR_LOCK(pv);                                                               \
            void *ptr_shifting = SOWR_CVECTOR_GET(pv, i);                                        \
            void *ptr_data = SOWR_CVECTOR_GET(pv, i + 1);                                        \
            size_t bytes_to_shift = pv->elem_size * (pv->length - i - 1);                        \
            memmove(ptr_retrieve, ptr_shifting, pv->elem_size);                                  \
            memmove(ptr_shifting, ptr_data, bytes_to_shift);                                     \
            pv->length--;                                                                        \
            SOWR_CVECTOR_UNLOCK(pv);                                                             \
        }                                                                                        \
    }                                                                                            \
}

#define SOWR_CVECTOR_PUSH(pv, ptr_element)                                                       \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        SOWR_CVECTOR_LOCK(pv);                                                                   \
        SOWR_CVECTOR_EXPAND_UNTIL(pv, pv->length + 1);                                           \
        void *ptr = &(pv->ptr[pv->length]);                                                      \
        memmove(ptr, ptr_element, pv->elem_size);                                                \
        pv->length++;                                                                            \
        SOWR_CVECTOR_UNLOCK(pv);                                                                 \
    }                                                                                            \
}

#define SOWR_CVECTOR_PUSH_CPY(pv, ptr_element)                                                   \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        SOWR_CVECTOR_LOCK(pv);                                                                   \
        SOWR_CVECTOR_EXPAND_UNTIL(pv, pv->length + 1);                                           \
        void *ptr = &(pv->ptr[pv->length]);                                                      \
        memcpy(ptr, ptr_element, pv->elem_size);                                                 \
        pv->length++;                                                                            \
        SOWR_CVECTOR_UNLOCK(pv);                                                                 \
    }                                                                                            \
}

#define SOWR_CVECTOR_POP(pv, ptr_retrieve)                                                       \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        SOWR_CVECTOR_LOCK(pv);                                                                   \
        if (ptr_retrieve)                                                                        \
        {                                                                                        \
            memmove(ptr_retrieve, &(pv->ptr[pv->length - 1]), pv->elem_size);                    \
        }                                                                                        \
        pv->length--;                                                                            \
        SOWR_CVECTOR_UNLOCK(pv);                                                                 \
    }                                                                                            \
}

#define SOWR_CVECTOR_SHRINK_TO_FIT(pv)                                                           \
{                                                                                                \
    if (atomic_load(&pv->usable))                                                                \
    {                                                                                            \
        SOWR_CVECTOR_LOCK(pv);                                                                   \
        if (pv->capacity > pv->length && pv->length)                                             \
        {                                                                                        \
            pv->ptr = sowr_ReAlloc(pv->length * pv->elem_size, pv->ptr);                         \
            pv->capacity = pv->length;                                                           \
        }                                                                                        \
    SOWR_CVECTOR_UNLOCK(pv);                                                                     \
    }                                                                                            \
}

#define SOWR_CVECTOR_DESTROY(pv)                                                                 \
{                                                                                                \
    SOWR_CVECTOR_LOCK(pv);                                                                       \
    if (pv->free_func)                                                                           \
    {                                                                                            \
        SOWR_CVECTOR_WALK(pv, pv->free_func);                                                    \
    }                                                                                            \
    sowr_HeapFree(pv->ptr);                                                                      \
    atomic_store(&pv->usable, false);                                                            \
    SOWR_CVECTOR_UNLOCK(pv);                                                                     \
    sowr_DestroyCriticalSection(pv->mtx);                                                        \
    sowr_HeapFree(pv->mtx);                                                                      \
    sowr_HeapFree(pv);                                                                           \
}

#endif // !SOWR_LIB_CONTAINER_CVECTOR_H
