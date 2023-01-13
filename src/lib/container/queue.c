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

#include "queue.h"

inline
blrt_Queue *
blrt_Queue_Create ( size_t elem_size, blrt_QueueFreeFunc free_func )
{
    return blrt_Vector_Create(elem_size, free_func);
}

inline
blrt_Queue
blrt_Queue_CreateS ( size_t elem_size, blrt_QueueFreeFunc free_func )
{
    return blrt_Vector_CreateS(elem_size, free_func);
}

inline
void *
blrt_Queue_First( const blrt_Queue *queue )
{
    return queue->ptr;
}

inline
void *
blrt_Queue_Find( const blrt_Queue *queue, const void *data, blrt_QueueCmpFunc cmp )
{
    return blrt_Vector_Find(queue, data, cmp);
}

inline
void
blrt_Queue_Expand( blrt_Queue *queue )
{
    blrt_Vector_Expand(queue);
}

inline
void
blrt_Queue_ExpandUntil( blrt_Queue *queue, size_t size )
{
    blrt_Vector_ExpandUntil(queue, size);
}

void
blrt_Queue_Eat( blrt_Queue *queue, blrt_QueueEatFunc func )
{
    if (!queue->length)
        return;

    for (size_t i = 0ULL; i < queue->length; i++)
        func(blrt_Vector_PtrAt(queue, i));
    blrt_Queue_Clear(queue);
}

inline
void
blrt_Queue_Clear( blrt_Queue *queue )
{
    blrt_Vector_Clear(queue);
}

inline
void
blrt_Queue_ShrinkToFit( blrt_Queue *queue )
{
    blrt_Vector_ShrinkToFit(queue);
}

inline
void
blrt_Queue_Push( blrt_Queue *queue, const void *elem )
{
    blrt_Vector_Push(queue, elem);
}

inline
void
blrt_Queue_Pop( blrt_Queue *queue, void *ptr_retrieve )
{
    blrt_Vector_Pop_Front(queue, ptr_retrieve);
}

inline
void
blrt_Queue_Destroy( blrt_Queue *queue )
{
    blrt_Vector_Destroy(queue);
}

inline
void
blrt_Queue_DestroyS( blrt_Queue *queue )
{
    blrt_Vector_DestroyS(queue);
}
