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

#include "vector.h"

sowr_Vector *
sowr_Vector_Create(size_t elem_size, const sowr_VecFreeFunc free_func)
{
    sowr_Vector *vec = sowr_HeapAlloc(sizeof(sowr_Vector));
    vec->length = 0;
    vec->capacity = 0;
    vec->elem_size = elem_size;
    vec->ptr = NULL;
    vec->free_func = free_func;
    return vec;
}

inline
void *
sowr_Vector_PtrAt(sowr_Vector *vec, size_t index)
{
    char *ptr = (char *)vec->ptr;
    for (size_t i = 0; i < index; i++)
        ptr += vec->elem_size;
    return ptr;
}

void
sowr_Vector_Expand(sowr_Vector *vec)
{
    if (!vec->capacity)
    {
        vec->capacity = 1;
        vec->ptr = sowr_HeapAlloc(vec->elem_size * 1);
    }
    else
    {
        vec->capacity *= 2;
        vec->ptr = sowr_ReAlloc(vec->elem_size * vec->capacity, vec->ptr);
    }
}

inline
void
sowr_Vector_ExpandUntil(sowr_Vector *vec, size_t size)
{
    while (vec->capacity < size)
        sowr_Vector_Expand(vec);
}

inline
void
sowr_Vector_Walk(sowr_Vector *vec, const sowr_VecWalkFunc func)
{
    for (char *ptr = (char *)vec->ptr; ptr != sowr_Vector_PtrAt(vec, vec->length); ptr += vec->elem_size)
        func(ptr);
}

void
sowr_Vector_Clear(sowr_Vector *vec)
{
    if (vec->free_func)
        sowr_Vector_Walk(vec, vec->free_func);
    vec->length = 0;
}

void
sowr_Vector_Insert(sowr_Vector *vec, const void *elem, size_t index)
{
    if (index >= vec->length)
        sowr_Vector_Push(vec, elem);
    else
    {
        sowr_Vector_ExpandUntil(vec, vec->length + 1);
        void *ptr_inserting = sowr_Vector_PtrAt(vec, vec->length);
        void *ptr_shifting = sowr_Vector_PtrAt(vec, vec->length + 1);
        size_t bytes_to_shift = vec->elem_size * (vec->length - index);
        memmove(ptr_shifting, ptr_inserting, bytes_to_shift);
        memcpy(ptr_inserting, elem, vec->elem_size);
        vec->length++;
    }
}

void
sowr_Vector_Replace(sowr_Vector *vec, const void *elem, size_t index)
{
    if (index >= vec->length)
        sowr_Vector_Push(vec, elem);
    else
    {
        void *ptr = sowr_Vector_PtrAt(vec, index);
        if (vec->free_func)
            vec->free_func(ptr);
        memcpy(ptr, elem, vec->elem_size);
    }
}

void
sowr_Vector_Delete(sowr_Vector *vec, size_t index)
{
    if (index >= vec->length)
        return;

    void *ptr_shifting = sowr_Vector_PtrAt(vec, index);
    if (vec->free_func)
        vec->free_func(ptr_shifting);

    void *ptr_data = sowr_Vector_PtrAt(vec, index + 1);
    size_t bytes_to_shift = vec->elem_size * (vec->length - index - 1);
    memmove(ptr_shifting, ptr_data, bytes_to_shift);
    vec->length--;
}

void
sowr_Vector_Take(sowr_Vector *vec, size_t index, void *ptr_retrieve)
{
    if (index >= vec->length)
        sowr_Vector_Pop(vec, ptr_retrieve);
    else
    {
        void *ptr_shifting = sowr_Vector_PtrAt(vec, index);
        void *ptr_data = sowr_Vector_PtrAt(vec, index + 1);
        size_t bytes_to_shift = vec->elem_size * (vec->length - index - 1);
        memcpy(ptr_retrieve, ptr_shifting, vec->elem_size);
        if (vec->free_func)
            vec->free_func(ptr_shifting);
        memmove(ptr_shifting, ptr_data, bytes_to_shift);
        vec->length--;
    }
}

void
sowr_Vector_Push(sowr_Vector *vec, const void *elem)
{
    sowr_Vector_ExpandUntil(vec, vec->length + 1);
    memcpy(sowr_Vector_PtrAt(vec, vec->length), elem, vec->elem_size);
    vec->length++;
}

void
sowr_Vector_Pop(sowr_Vector *vec, void *ptr_retrieve)
{
    if (ptr_retrieve)
        memcpy(ptr_retrieve, sowr_Vector_PtrAt(vec, vec->length), vec->elem_size);
    if (vec->free_func)
        vec->free_func(sowr_Vector_PtrAt(vec, vec->length));
    vec->length--;
}

void
sowr_Vector_ShrinkToFit(sowr_Vector *vec)
{
    if (vec->capacity > vec->length && vec->length)
    {
        vec->ptr = sowr_ReAlloc(vec->length * vec->elem_size, vec->ptr);
        vec->capacity = vec->length;
    }
}

void
sowr_Vector_Destroy(sowr_Vector *vec)
{
    sowr_Vector_Clear(vec);
    sowr_HeapFree(vec->ptr);
    sowr_HeapFree(vec);
}
