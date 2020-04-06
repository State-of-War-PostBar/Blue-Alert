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

#ifndef SOWR_LIB_CONTAINER_LINKED_LIST_H
#define SOWR_LIB_CONTAINER_LINKED_LIST_H

#include <pch.h>

typedef void (*sowr_LinkedListFreeFunc)(void *);
typedef void (*sowr_LinkedListWalkFunc)(void *);
typedef bool (*sowr_LinkedListCmpFunc)(const void *, const void *);

typedef struct sowr_Linked_List_Node
{
    void *data;
    struct sowr_Linked_List_Node *next;
} sowr_Linked_List_Node;

typedef struct
{
    sowr_Linked_List_Node *next;
    size_t elem_size;
    size_t length;
    sowr_LinkedListFreeFunc free_func;
} sowr_Linked_List;

sowr_Linked_List *
sowr_LinkedList_Create(size_t, const sowr_LinkedListFreeFunc);

void
sowr_LinkedList_Walk(sowr_Linked_List *, const sowr_LinkedListWalkFunc);

void
sowr_LinkedList_Clear(sowr_Linked_List *);

sowr_Linked_List_Node *
sowr_LinkedList_Insert_Mov(sowr_Linked_List *, void *);

sowr_Linked_List_Node *
sowr_LinkedList_Insert_Cpy(sowr_Linked_List *, const void *);

void
sowr_LinkedList_Pop(sowr_Linked_List *);

void
sowr_LinkedList_PopNF(sowr_Linked_List *);

sowr_Linked_List_Node *
sowr_LinkedList_Find(const sowr_Linked_List *, const void *, const sowr_LinkedListCmpFunc);

sowr_Linked_List_Node *
sowr_LinkedList_Back(const sowr_Linked_List *);

void
sowr_LinkedList_Delete(sowr_Linked_List *, void *, const sowr_LinkedListCmpFunc);

void
sowr_LinkedList_DeleteNF(sowr_Linked_List *, void *, const sowr_LinkedListCmpFunc);

void
sowr_LinkedList_Destroy(sowr_Linked_List *);

#endif //!SOWR_LIB_CONTAINER_LINKED_LIST_H
