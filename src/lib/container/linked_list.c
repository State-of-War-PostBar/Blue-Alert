/*************************************************************************************************
*                                                                                                *
*                                    State of War: Remastered                                    *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
* A free, open-source software project recreating an old game.                                   *
* (c) 2017 - 2020 State of War Baidu Postbar, some rights reserved.                              *
*                                                                                                *
* State of War: Remastered is a free software. You can freely do whatever you want with it       *
* under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)         *
* published by mhtvsSFrpHdE.                                                                     *
* By the time this line is written, the version of the license document is 1, but you may use    *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
* State of War: Remastered is created, intended to be useful, but without any warranty.          *
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
    list->elem_size = elem_size;
    list->length = 0ULL;
    list->free_func = free_func;
    list->head = NULL;
    return list;
}

sowr_LinkedList
sowr_LinkedList_CreateS(size_t elem_size, const sowr_LinkedListFreeFunc free_func)
{
    sowr_LinkedList list =
    {
        .elem_size = elem_size,
        .length = 0ULL,
        .free_func = free_func,
        .head = NULL
    };
    return list;
}

void
sowr_LinkedList_Walk(sowr_LinkedList *list, const sowr_LinkedListWalkFunc func)
{
    if (!list->length)
        return;

    for (sowr_LinkedListNode *iter = list->head; iter; iter = iter->next)
        func(iter->data);
}

void
sowr_LinkedList_Clear(sowr_LinkedList *list)
{
    if (!list->length)
        return;

    sowr_LinkedListNode *iter = list->head, *save = NULL;
    while (iter)
    {
        save = iter->next;
        if (list->free_func)
            list->free_func(iter->data);
        sowr_HeapFree(iter->data);
        sowr_HeapFree(iter);
        iter = save;
    }
    list->head = NULL;
    list->length = 0;
}

void
sowr_LinkedList_Insert(sowr_LinkedList *list, const void *elem)
{
    sowr_LinkedListNode *node = sowr_HeapAlloc(sizeof(sowr_LinkedListNode));
    node->data = sowr_HeapAlloc(list->elem_size);
    memcpy(node->data, elem, list->elem_size);
    node->next = list->length ? list->head : NULL;
    list->head = node;
    list->length++;
}

void
sowr_LinkedList_Pop(sowr_LinkedList *list)
{
    if (!list->length)
        return;

    sowr_LinkedListNode *head = list->head, *next = list->head->next;
    if (list->free_func)
        list->free_func(head->data);
    sowr_HeapFree(head->data);
    sowr_HeapFree(head);
    if (next)
        list->head = next;

    list->length--;
}

void
sowr_LinkedList_PopN(sowr_LinkedList *list, void **ptr_retrieve)
{
    if (!list->length)
        return;

    sowr_LinkedListNode *head = list->head, *next = list->head->next;
    *ptr_retrieve = head->data;
    sowr_HeapFree(head);
    if (next)
        list->head = next;

    list->length--;
}

sowr_LinkedListNode *
sowr_LinkedList_Find(const sowr_LinkedList *list, const void *elem, const sowr_LinkedListCmpFunc cmp)
{
    if (!list->length)
        return NULL;

    for (sowr_LinkedListNode *iter = list->head; iter; iter = iter->next)
        if (cmp(iter->data, elem))
            return iter;

    return NULL;
}

size_t
sowr_LinkedList_Delete(sowr_LinkedList *list, const void *elem, const sowr_LinkedListCmpFunc cmp)
{
    if (!list->length)
        return 0ULL;

    size_t count = 0ULL;
    sowr_LinkedListNode *iter = list->head, *prev = NULL, *next = NULL;

    while (iter)
    {
        next = iter->next;
        if (cmp(iter->data, elem))
        {
            if (prev)
                prev->next = next;
            if (list->free_func)
                list->free_func(iter->data);
            sowr_HeapFree(iter->data);
            sowr_HeapFree(iter);
            count++;
            list->length--;
            if (!list->length)
                return count;
        }
        prev = iter;
        iter = next;
    }

    return count;
}

size_t
sowr_LinkedList_DeleteN(sowr_LinkedList *list, const void *elem, const sowr_LinkedListCmpFunc cmp, void **ptr_retrieve)
{
    if (!list->length)
        return 0ULL;

    size_t count = 0ULL;
    sowr_LinkedListNode *iter = list->head, *prev = NULL, *next = NULL;
    while (iter)
    {
        next = iter->next;
        if (cmp(iter->data, elem))
        {
            if (prev)
                prev->next = next;
            if (ptr_retrieve)
                *ptr_retrieve = iter->data;
            sowr_HeapFree(iter);
            count++;
            list->length--;
        }
        prev = iter;
        iter = next;
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
