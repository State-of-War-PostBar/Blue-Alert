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

#include "linked_list.h"

#include "../memory/heap_memory.h"

blrt_LinkedList *
blrt_LinkedList_Create( blrt_LinkedListFreeFunc free_func )
{
    blrt_LinkedList *list = blrt_HeapAlloc(sizeof(blrt_LinkedList));
    list->length = 0ULL;
    list->free_func = free_func;
    list->head = (blrt_LinkedListNode){ 0 };
    return list;
}

blrt_LinkedList
blrt_LinkedList_CreateS( blrt_LinkedListFreeFunc free_func )
{
    blrt_LinkedList list =
    {
        .length = 0ULL,
        .free_func = free_func,
        .head = (blrt_LinkedListNode){ 0 }
    };
    return list;
}

void
blrt_LinkedList_Walk( blrt_LinkedList *list, blrt_LinkedListWalkFunc func )
{
    if (!list->length)
        return;

    for (blrt_LinkedListNode *iter = &(list->head); iter; iter = iter->next)
        func(iter->data);
}

void
blrt_LinkedList_Clear( blrt_LinkedList *list )
{
    if (!list->length)
        return;

    blrt_LinkedListNode *iter = &(list->head), *next = NULL;
    while (iter)
    {
        next = iter->next;
        if (list->free_func)
            list->free_func(iter->data);
        blrt_HeapFree(iter->data);
        if (iter != &(list->head))
            blrt_HeapFree(iter);
        iter = next;
    }
    list->head = (blrt_LinkedListNode){ 0 };
    list->length = 0ULL;
}

void
blrt_LinkedList_Insert( blrt_LinkedList *list, size_t data_size, const void *data )
{
    blrt_LinkedListNode node;
    node.data = blrt_HeapAlloc(data_size);
    node.data_size = data_size;
    memcpy(node.data, data, data_size);
    if (!list->length)
        node.next = NULL;
    else
    {
        blrt_LinkedListNode *old_head = blrt_HeapAlloc(sizeof(blrt_LinkedListNode));
        memcpy(old_head, &(list->head), sizeof(blrt_LinkedListNode));
        node.next = old_head;
    }
    list->head = node;
    list->length++;
}

void
blrt_LinkedList_Pop( blrt_LinkedList *list, void *ptr_retrieve )
{
    if (!list->length)
        return;

    if (ptr_retrieve)
        memcpy(ptr_retrieve, list->head.data, list->head.data_size);
    else if (list->free_func)
        list->free_func(list->head.data);
    blrt_HeapFree(list->head.data);

    if (list->head.next)
    {

        blrt_LinkedListNode *next = list->head.next;
        list->head = *next;
        blrt_HeapFree(next);
    }

    list->length--;
}

blrt_LinkedListNode *
blrt_LinkedList_Find( const blrt_LinkedList *list, const void *data, blrt_LinkedListCmpFunc cmp )
{
    if (!list->length)
        return NULL;

    for (const blrt_LinkedListNode *iter = &(list->head); iter; iter = iter->next)
        if (cmp(iter->data, data))
            return (blrt_LinkedListNode *)iter;

    return NULL;
}

size_t
blrt_LinkedList_Take( blrt_LinkedList *list, const void *data, blrt_LinkedListCmpFunc cmp, void *ptr_retrieve )
{
    if (!list->length)
        return 0ULL;

    size_t count = 0ULL;
    blrt_LinkedListNode *iter = &(list->head), *previous = NULL;
    bool copied = false;

    while (iter)
    {
        if (cmp(iter->data, data))
        {
            if (ptr_retrieve && !copied)
            {
                memcpy(ptr_retrieve, iter->data, iter->data_size);
                copied = true;
            }
            else if (list->free_func)
                list->free_func(iter->data);
            blrt_HeapFree(iter->data);

            if (previous)
                previous->next = iter->next;
            else
            {
                blrt_LinkedListNode *next = iter->next;
                list->head = *next;
                blrt_HeapFree(next);
            }

            count++;
            list->length--;

            if (!list->length)
                return count;

            if (previous)
                iter = previous->next;
            else
                iter = &(list->head);
        }
        else
        {
            previous = iter;
            iter = iter->next;
        }
    }
    return count;
}

void
blrt_LinkedList_Destroy( blrt_LinkedList *list )
{
    blrt_LinkedList_Clear(list);
    blrt_HeapFree(list);
}

void
blrt_LinkedList_DestroyS( blrt_LinkedList *list )
{
    blrt_LinkedList_Clear(list);
}
