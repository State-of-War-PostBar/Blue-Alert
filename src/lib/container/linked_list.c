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

sowr_Linked_List *
sowr_LinkedList_Create(size_t elem_size, const sowr_LinkedListFreeFunc free_func)
{
    sowr_Linked_List *list = sowr_HeapAlloc(sizeof(sowr_Linked_List));
    list->next = NULL;
    list->elem_size = elem_size;
    list->length = 0;
    list->free_func = free_func;
    return list;
}

void
sowr_LinkedList_Walk(sowr_Linked_List *list, const sowr_LinkedListWalkFunc func)
{
    if (!list->next)
        return;

    for (sowr_Linked_List_Node *iter = list->next; iter; iter = iter->next)
        func(iter->data);
}

void
sowr_LinkedList_Clear(sowr_Linked_List *list)
{
    sowr_Linked_List_Node *iter = list->next, *save = NULL;
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
}

sowr_Linked_List_Node *
sowr_LinkedList_Insert_Mov(sowr_Linked_List *list, void *elem)
{
    sowr_Linked_List_Node *node = sowr_HeapAlloc(sizeof(sowr_Linked_List_Node));
    node->data = sowr_HeapAlloc(list->elem_size);
    memmove(node->data, elem, list->elem_size);
    node->next = list->length ? list->next : NULL;
    list->next = node;
    list->length++;

    return node;
}

sowr_Linked_List_Node *
sowr_LinkedList_Insert_Cpy(sowr_Linked_List *list, const void *elem)
{
    sowr_Linked_List_Node *node = sowr_HeapAlloc(sizeof(sowr_Linked_List_Node));
    node->data = sowr_HeapAlloc(list->elem_size);
    memcpy(node->data, elem, list->elem_size);
    node->next = list->next ? list->next : NULL;
    list->next = node;
    list->length++;

    return node;
}

void
sowr_LinkedList_Pop(sowr_Linked_List *list)
{
    if (!list->length)
        return;

    sowr_Linked_List_Node *next = list->next;

    if (list->next->next)
    {
        sowr_Linked_List_Node *save = list->next->next;
        list->next = save;
    }
    else
        list->next = NULL;

    if(list->free_func)
        list->free_func(next->data);

    sowr_HeapFree(next);
}

void
sowr_LinkedList_PopNF(sowr_Linked_List *list)
{
    if (!list->length)
        return;

    sowr_Linked_List_Node *next = list->next;

    if (list->next->next)
    {
        sowr_Linked_List_Node *save = list->next->next;
        list->next = save;
    }
    else
        list->next = NULL;

    sowr_HeapFree(next);
}

sowr_Linked_List_Node *
sowr_LinkedList_Find(const sowr_Linked_List *list, const void *elem, const sowr_LinkedListCmpFunc cmp)
{
    for (sowr_Linked_List_Node *iter = list->next; iter; iter = iter->next)
        if (cmp(iter->data, elem))
            return iter;

    return NULL;
}

sowr_Linked_List_Node *
sowr_LinkedList_Back(const sowr_Linked_List *list)
{
    sowr_Linked_List_Node *iter = list->next, *save = NULL;
    while (iter)
    {
        save = iter;
        iter = iter->next;
    }

    return save;
}

int
sowr_LinkedList_Delete(sowr_Linked_List *list, const void *elem, const sowr_LinkedListCmpFunc cmp)
{
    sowr_Linked_List_Node *iter = list->next, *prev = NULL, *next = NULL;
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
            return 1;
        }
        prev = iter;
        iter = iter->next;
    }

    return 0;
}

int
sowr_LinkedList_DeleteNF(sowr_Linked_List *list, void *elem, const sowr_LinkedListCmpFunc cmp)
{
    sowr_Linked_List_Node *iter = list->next, *prev = NULL, *next = NULL;
    while (iter)
    {
        next = iter->next;
        if (cmp(iter->data, elem))
        {
            if (prev)
                prev->next = next;
            sowr_HeapFree(iter->data);
            sowr_HeapFree(iter);
            return 1;
        }
        prev = iter;
        iter = iter->next;
    }

    return 0;
}

void
sowr_LinkedList_Destroy(sowr_Linked_List *list)
{
    sowr_LinkedList_Clear(list);
    sowr_HeapFree(list);
}
