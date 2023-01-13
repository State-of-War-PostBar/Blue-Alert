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

#include "vector.h"

#include "../memory/heap_memory.h"

blrt_Vector *
blrt_Vector_Create( size_t elem_size, blrt_VectorFreeFunc free_func )
{
    blrt_Vector *vec = blrt_HeapAlloc(sizeof(blrt_Vector));
    vec->length = 0ULL;
    vec->capacity = 0ULL;
    vec->elem_size = elem_size;
    vec->free_func = free_func;
    vec->ptr = NULL;
    return vec;
}

blrt_Vector
blrt_Vector_CreateS( size_t elem_size, blrt_VectorFreeFunc free_func )
{
    blrt_Vector vec =
    {
        .length = 0ULL,
        .capacity = 0ULL,
        .elem_size = elem_size,
        .free_func = free_func,
        .ptr = NULL
    };
    return vec;
}

inline
void *
blrt_Vector_First( const blrt_Vector *vec )
{
    return vec->ptr;
}

inline
void *
blrt_Vector_Last( const blrt_Vector *vec )
{
    if (!vec->length)
        return NULL;

    return (void *)((char *)(vec->ptr) + vec->elem_size * (vec->length - 1ULL));
}

inline
void *
blrt_Vector_PtrAt( const blrt_Vector *vec, size_t index )
{
    return (void *)((char *)(vec->ptr) + vec->elem_size * index);
}

void *
blrt_Vector_Find( const blrt_Vector *vec, const void *elem, blrt_VectorCmpFunc cmp )
{
    if (!vec->length)
        return NULL;

    char *ptr = vec->ptr;
    for (size_t i = 0ULL; i < vec->length; i++, ptr += vec->elem_size)
        if (cmp((void *)ptr, elem))
            return ptr;

    return NULL;
}

void
blrt_Vector_Expand( blrt_Vector *vec )
{
    if (!vec->capacity)
    {
        vec->capacity = 1ULL;
        vec->ptr = blrt_HeapAlloc(vec->elem_size);
    }
    else
    {
        vec->capacity *= 2ULL;
        vec->ptr = blrt_ReAlloc(vec->elem_size * vec->capacity, vec->ptr);
    }
}

inline
void
blrt_Vector_ExpandUntil( blrt_Vector *vec, size_t size )
{
    while (vec->capacity < size)
        blrt_Vector_Expand(vec);
}

inline
void
blrt_Vector_ShrinkToFit( blrt_Vector *vec )
{
    if (vec->capacity > vec->length && vec->length)
    {
        vec->ptr = blrt_ReAlloc(vec->length * vec->elem_size, vec->ptr);
        vec->capacity = vec->length;
    }
    else if (vec->capacity && !vec->length)
    {
        blrt_HeapFree(vec->ptr);
        vec->capacity = 0ULL;
    }
}

void
blrt_Vector_Walk( blrt_Vector *vec, blrt_VectorWalkFunc func )
{
    if (!vec->length)
        return;

    char *ptr = vec->ptr;
    for (size_t i = 0ULL; i < vec->length; i++, ptr += vec->elem_size)
        func((void *)ptr);
}

void
blrt_Vector_Clear( blrt_Vector *vec )
{
    if (vec->free_func)
        blrt_Vector_Walk(vec, vec->free_func);
    vec->length = 0ULL;
}

void
blrt_Vector_Insert( blrt_Vector *vec, size_t index, const void *elem )
{
    if (index >= vec->length)
        blrt_Vector_Push(vec, elem);
    else
    {
        blrt_Vector_ExpandUntil(vec, vec->length + 1ULL);
        void *ptr_inserting = blrt_Vector_PtrAt(vec, index);
        void *ptr_shifting = blrt_Vector_PtrAt(vec, index + 1ULL);
        size_t bytes_to_shift = vec->elem_size * (vec->length - index);
        memmove(ptr_shifting, ptr_inserting, bytes_to_shift);
        memcpy(ptr_inserting, elem, vec->elem_size);
        vec->length++;
    }
}

void
blrt_Vector_Replace( blrt_Vector *vec, size_t index, const void *elem )
{
    if (index >= vec->length)
        blrt_Vector_Push(vec, elem);
    else
    {
        void *ptr = blrt_Vector_PtrAt(vec, index);
        if (vec->free_func)
            vec->free_func(ptr);
        memcpy(ptr, elem, vec->elem_size);
    }
}

void
blrt_Vector_Delete( blrt_Vector *vec, size_t index )
{
    if (index >= vec->length)
        return;

    void *ptr_shifting = blrt_Vector_PtrAt(vec, index);
    if (vec->free_func)
        vec->free_func(ptr_shifting);
    void *ptr_data = blrt_Vector_PtrAt(vec, index + 1ULL);
    size_t bytes_to_shift = vec->elem_size * (vec->length - index - 1ULL);
    memmove(ptr_shifting, ptr_data, bytes_to_shift);
    vec->length--;
}

void
blrt_Vector_Take( blrt_Vector *vec, size_t index, void *ptr_retrieve )
{
    if (!vec->length)
        return;
    if (index >= vec->length)
        blrt_Vector_Pop(vec, ptr_retrieve);
    else
    {
        void *ptr_shifting = blrt_Vector_PtrAt(vec, index);
        void *ptr_data = blrt_Vector_PtrAt(vec, index + 1ULL);
        size_t bytes_to_shift = vec->elem_size * (vec->length - index - 1ULL);
        if (ptr_retrieve)
            memcpy(ptr_retrieve, ptr_shifting, vec->elem_size);
        else if (vec->free_func)
            vec->free_func(ptr_shifting);
        memmove(ptr_shifting, ptr_data, bytes_to_shift);
        vec->length--;
    }
}

void
blrt_Vector_Push( blrt_Vector *vec, const void *elem )
{
    blrt_Vector_ExpandUntil(vec, vec->length + 1ULL);
    memcpy(blrt_Vector_PtrAt(vec, vec->length), elem, vec->elem_size);
    vec->length++;
}

void
blrt_Vector_Pop( blrt_Vector *vec, void *ptr_retrieve )
{
    if (!vec->length)
        return;
    if (ptr_retrieve)
        memcpy(ptr_retrieve, blrt_Vector_PtrAt(vec, vec->length), vec->elem_size);
    else if (vec->free_func)
        vec->free_func(blrt_Vector_PtrAt(vec, vec->length));
    vec->length--;
}

void
blrt_Vector_Push_Front( blrt_Vector *vec, const void *elem )
{
    blrt_Vector_ExpandUntil(vec, vec->length + 1ULL);
    memmove(blrt_Vector_PtrAt(vec, 1ULL), vec->ptr, vec->elem_size * vec->length);
    memcpy(vec->ptr, elem, vec->elem_size);
    vec->length++;
}

void
blrt_Vector_Pop_Front( blrt_Vector *vec, void *ptr_retrieve )
{
    if (!vec->length)
        return;
    if (ptr_retrieve)
        memcpy(ptr_retrieve, vec->ptr, vec->elem_size);
    else if (vec->free_func)
        vec->free_func(vec->ptr);
    vec->length--;
    memmove(vec->ptr, blrt_Vector_PtrAt(vec, 1ULL), vec->elem_size * vec->length);
}

void
blrt_Vector_Destroy( blrt_Vector *vec )
{
    blrt_Vector_Clear(vec);
    if (vec->capacity)
        blrt_HeapFree(vec->ptr);
    blrt_HeapFree(vec);
}

void
blrt_Vector_DestroyS( blrt_Vector *vec )
{
    blrt_Vector_Clear(vec);
    if (vec->capacity)
        blrt_HeapFree(vec->ptr);
}
