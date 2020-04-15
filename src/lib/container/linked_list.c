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

#include "linked_list.h"

#include "../memory/heap_memory.h"

sowr_LinkedList *
sowr_LinkedList_Create(size_t elem_size, const sowr_LinkedListFreeFunc free_func)
{
    sowr_LinkedList *list = sowr_HeapAlloc(sizeof(sowr_LinkedList));
    list->next = NULL;
    list->elem_size = elem_size;
    list->length = 0;
    list->free_func = free_func;
    return list;
}

sowr_LinkedList
sowr_LinkedList_CreateS(size_t elem_size, const sowr_LinkedListFreeFunc free_func)
{
    sowr_LinkedList list =
    {
        .next = NULL,
        .elem_size = elem_size,
        .length = 0,
        .free_func = free_func
    };
    return list;
}

void
sowr_LinkedList_Walk(const sowr_LinkedList *list, const sowr_LinkedListWalkFunc func)
{
    if (!list->next)
        return;

    for (sowr_LinkedListNode *iter = list->next; iter; iter = iter->next)
        func(iter->data);
}

void
sowr_LinkedList_Clear(sowr_LinkedList *list)
{
    if (!list->length)
        return;

    sowr_LinkedListNode *iter = list->next, *save = NULL;
    while (iter)
    {
        save = iter->next;
        if (list->free_func)
            list->free_func(iter->data);
        sowr_HeapFree(iter->data);
        sowr_HeapFree(iter);
        iter = save;
    }
    list->next = NULL;
    list->length = 0;
}

sowr_LinkedListNode *
sowr_LinkedList_Insert(sowr_LinkedList *list, const void *elem)
{
    sowr_LinkedListNode *node = sowr_HeapAlloc(sizeof(sowr_LinkedListNode));
    node->data = sowr_HeapAlloc(list->elem_size);
    memcpy(node->data, elem, list->elem_size);
    node->next = list->length ? list->next : NULL;
    list->next = node;
    list->length++;

    return node;
}

void
sowr_LinkedList_Pop(sowr_LinkedList *list)
{
    if (!list->length)
        return;

    sowr_LinkedListNode *next = list->next;

    if (list->next->next)
    {
        sowr_LinkedListNode *save = list->next->next;
        list->next = save;
    }
    else
        list->next = NULL;

    if(list->free_func)
        list->free_func(next->data);

    list->length--;
    sowr_HeapFree(next);
}

void
sowr_LinkedList_PopNF(sowr_LinkedList *list, void **ptr_retrieve)
{
    if (!list->length)
    {
        *ptr_retrieve = NULL;
        return;
    }

    sowr_LinkedListNode *next = list->next;

    if (list->next->next)
    {
        sowr_LinkedListNode *save = list->next->next;
        list->next = save;
    }
    else
        list->next = NULL;

    if (ptr_retrieve)
        *ptr_retrieve = next->data;

    list->length--;
    sowr_HeapFree(next);
}

sowr_LinkedListNode *
sowr_LinkedList_Find(const sowr_LinkedList *list, const void *elem, const sowr_LinkedListCmpFunc cmp)
{
    for (sowr_LinkedListNode *iter = list->next; iter; iter = iter->next)
        if (cmp(iter->data, elem))
            return iter;

    return NULL;
}

sowr_LinkedListNode *
sowr_LinkedList_Back(const sowr_LinkedList *list)
{
    sowr_LinkedListNode *iter = list->next, *save = NULL;
    while (iter)
    {
        save = iter;
        iter = iter->next;
    }

    return save;
}

size_t
sowr_LinkedList_Delete(sowr_LinkedList *list, const void *elem, const sowr_LinkedListCmpFunc cmp)
{
    size_t count = 0;
    sowr_LinkedListNode *iter = list->next, *prev = NULL, *next = NULL, *save = NULL;
    while (iter)
    {
        next = iter->next;
        if (cmp(iter->data, elem))
        {
            if (prev)
                prev->next = next;
            if (list->free_func)
                list->free_func(iter->data);
            save = iter;
            sowr_HeapFree(iter->data);
            sowr_HeapFree(iter);
            count++;
            list->length--;
            if (!list->length)
                return count;
            iter = save;
        }
        prev = iter;
        iter = iter->next;
    }

    return count;
}

size_t
sowr_LinkedList_DeleteNF(sowr_LinkedList *list, const void *elem, const sowr_LinkedListCmpFunc cmp, void **ptr_retrieve)
{
    size_t count = 0;
    sowr_LinkedListNode *iter = list->next, *prev = NULL, *next = NULL, *save = NULL;
    while (iter)
    {
        next = iter->next;
        if (cmp(iter->data, elem))
        {
            if (prev)
                prev->next = next;
            if (ptr_retrieve)
                *ptr_retrieve = iter->data;
            save = iter;
            sowr_HeapFree(iter);
            count++;
            list->length--;
            if (!next)
                return count;
            iter = save;
        }
        prev = iter;
        iter = iter->next;
    }

    if (!count)
        *ptr_retrieve = NULL;

    return count;
}

void
sowr_LinkedList_Destroy(sowr_LinkedList *list)
{
    sowr_LinkedList_Clear(list);
    sowr_HeapFree(list);
}

void
sowr_LinkedList_DestroyS(sowr_LinkedList *list)
{
    sowr_LinkedList_Clear(list);
}
