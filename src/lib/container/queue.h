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

#ifndef BLRT_LIB_CONTAINER_QUEUE_H
#define BLRT_LIB_CONTAINER_QUEUE_H

#include <pch.h>

#include "vector.h"

typedef void (*blrt_QueueFreeFunc)( void * );
typedef void (*blrt_QueueEatFunc)( void * );
typedef bool (*blrt_QueueCmpFunc)( const void *, const void * );

typedef blrt_Vector blrt_Queue;

///
/// \brief Create a queue
///
/// Create a queue.
///
/// \param elem_size Size of queue's elements
/// \param free_func Function to call when the queue frees an element
///
/// \return Created queue
///
blrt_Queue *
blrt_Queue_Create ( size_t elem_size, blrt_QueueFreeFunc free_func );

///
/// \brief Create a queue
///
/// Create a queue by stack.<BR />
/// <B>The created queue must be freed by \a blrt_Queue_DestroyS().</B>
///
/// \param elem_size Size of queue's elements
/// \param free_func Function to call when the queue frees an element
///
/// \return Created queue
///
blrt_Queue
blrt_Queue_CreateS ( size_t elem_size, blrt_QueueFreeFunc free_func );

///
/// \brief Get the first element
///
/// Get the first element of the queue.
///
/// \param queue Queue
///
/// \return Pointer to the first element
///
void *
blrt_Queue_First( const blrt_Queue *queue );

///
/// \brief Find an element in the queue
///
/// Search for an element in the given queue.
///
/// \param queue Queue
/// \param data Data to search
/// \param cmp Comparision function
///
/// \return Pointer to the found element, NULL otherwise.
///
void *
blrt_Queue_Find( const blrt_Queue *queue, const void *data, blrt_QueueCmpFunc cmp );

///
/// \brief Expand the queue
///
/// Expand the queue, usually doubling its capacity unless the length is 0.
///
/// \param queue Queue to expand
///
void
blrt_Queue_Expand( blrt_Queue *queue );

///
/// \brief Expand the queue
///
/// Expand the queue until its capacity reaches the set limit.
///
/// \param queue Queue to expand
/// \param size Target size for expanding
///
void
blrt_Queue_ExpandUntil( blrt_Queue *queue, size_t size );

///
/// \brief Eat the queue
///
/// Eat the elements of the queue. After the operations all elements will be freed.
///
/// \param queue Queue to eat
/// \param func Function for eating
///
void
blrt_Queue_Eat( blrt_Queue *queue, blrt_QueueEatFunc func );

///
/// \brief Clear out a queue
///
/// Clear the content of a queue. Its capacity is unchanged.
///
/// \param queue Queue to clear
///
void
blrt_Queue_Clear( blrt_Queue *queue );

///
/// \brief Shrink the queue
///
/// Shrink the queue to just enough to fit its contents.
///
/// \param queue Queue to shrink
///
void
blrt_Queue_ShrinkToFit( blrt_Queue *queue );

///
/// \brief Push an element
///
/// Push an element into the end of queue.
///
/// \param queue Queue to push into
/// \param elem Element to be pushed
///
void
blrt_Queue_Push( blrt_Queue *queue, const void *elem );

///
/// \brief Pop the first element of queue
///
/// Pop the first element of queue into the pointer.
/// Whether ptr_retrieve is NULL or not, the first element will be overriden.
///
/// \param queue Queue to pop out
/// \param ptr_retrieve Pointer to retrieve the result
///
void
blrt_Queue_Pop( blrt_Queue *queue, void *ptr_retrieve );

///
/// \brief Destroy a queue
///
/// Destroy a queue, freeing all its elements.
///
/// \param queue Queue to destroy
///
void
blrt_Queue_Destroy( blrt_Queue *queue );

///
/// \brief Destroy a queue
///
/// Destroy a queue created by \a blrt_Queue_CreateS(), freeing all its elements.
///
/// \param queue Queue to destroy
///
void
blrt_Queue_DestroyS( blrt_Queue *queue );

#endif //!BLRT_LIB_CONTAINER_QUEUE_H
