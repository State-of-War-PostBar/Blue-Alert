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

#include "queue.h"

inline
sowr_Queue *
sowr_Queue_Create ( size_t elem_size, sowr_QueueFreeFunc free_func )
{
    return sowr_Vector_Create(elem_size, free_func);
}

inline
sowr_Queue
sowr_Queue_CreateS ( size_t elem_size, sowr_QueueFreeFunc free_func )
{
    return sowr_Vector_CreateS(elem_size, free_func);
}

inline
void *
sowr_Queue_First( const sowr_Queue *queue )
{
    return queue->ptr;
}

inline
void
sowr_Queue_Expand( sowr_Queue *queue )
{
    sowr_Vector_Expand(queue);
}

inline
void
sowr_Queue_ExpandUntil( sowr_Queue *queue, size_t size )
{
    sowr_Vector_ExpandUntil(queue, size);
}

void
sowr_Queue_Eat( sowr_Queue *queue, sowr_QueueEatFunc func )
{
    if (!queue->length)
        return;

    for (size_t i = 0ULL; i < queue->length; i++)
        func(sowr_Vector_PtrAt(queue, i));
    sowr_Queue_Clear(queue);
}

inline
void
sowr_Queue_Clear( sowr_Queue *queue )
{
    sowr_Vector_Clear(queue);
}

inline
void
sowr_Queue_ShrinkToFit( sowr_Queue *queue )
{
    sowr_Vector_ShrinkToFit(queue);
}

inline
void
sowr_Queue_Push( sowr_Queue *queue, const void *elem )
{
    sowr_Vector_Push(queue, elem);
}

inline
void
sowr_Queue_Pop( sowr_Queue *queue, void *ptr_retrieve )
{
    sowr_Vector_Pop_Front(queue, ptr_retrieve);
}

inline
void
sowr_Queue_Destroy( sowr_Queue *queue )
{
    sowr_Vector_Destroy(queue);
}

inline
void
sowr_Queue_DestroyS( sowr_Queue *queue )
{
    sowr_Vector_DestroyS(queue);
}
