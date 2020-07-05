/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (c) 2017 - 2020 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                                   published by mhtvsSFrpHdE.                                   *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
* any later version of the document released by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.  *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*       You should have received a copy of the license along with the source code of this        *
*  program. If not, please see https://github.com/mhtvsSFrpHdE/ipcui/blob/master/LICENSE_JDBM.   *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                         https://github.com/State-of-War-PostBar/sowr.                          *
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#ifndef SOWR_LIB_CONTAINER_QUEUE_H
#define SOWR_LIB_CONTAINER_QUEUE_H

#include <pch.h>

#include "vector.h"

typedef void (*sowr_QueueFreeFunc)( void * );
typedef void (*sowr_QueueEatFunc)( void * );

typedef sowr_Vector sowr_Queue;

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
sowr_Queue *
sowr_Queue_Create ( size_t elem_size, sowr_QueueFreeFunc free_func );

///
/// \brief Create a queue
///
/// Create a queue by stack.<BR />
/// <B>The created queue must be freed by \a sowr_Queue_DestroyS().</B>
///
/// \param elem_size Size of queue's elements
/// \param free_func Function to call when the queue frees an element
///
/// \return Created queue
///
sowr_Queue
sowr_Queue_CreateS ( size_t elem_size, sowr_QueueFreeFunc free_func );

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
sowr_Queue_First( const sowr_Queue *queue );

///
/// \brief Expand the queue
///
/// Expand the queue, usually doubling its capacity unless the length is 0.
///
/// \param queue Queue to expand
///
void
sowr_Queue_Expand( sowr_Queue *queue );

///
/// \brief Expand the queue
///
/// Expand the queue until its capacity reaches the set limit.
///
/// \param queue Queue to expand
/// \param size Target size for expanding
///
void
sowr_Queue_ExpandUntil( sowr_Queue *queue, size_t size );

///
/// \brief Eat the queue
///
/// Eat the elements of the queue. After the operations all elements will be freed.
///
/// \param queue Queue to eat
/// \param func Function for eating
///
void
sowr_Queue_Eat( sowr_Queue *queue, sowr_QueueEatFunc func );

///
/// \brief Clear out a queue
///
/// Clear the content of a queue. Its capacity is unchanged.
///
/// \param queue Queue to clear
///
void
sowr_Queue_Clear( sowr_Queue *queue );

///
/// \brief Shrink the queue
///
/// Shrink the queue to just enough to fit its contents.
///
/// \param queue Queue to shrink
///
void
sowr_Queue_ShrinkToFit( sowr_Queue *queue );

///
/// \brief Push an element
///
/// Push an element into the end of queue.
///
/// \param queue Queue to push into
/// \param elem Element to be pushed
///
void
sowr_Queue_Push( sowr_Queue *queue, const void *elem );

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
sowr_Queue_Pop( sowr_Queue *queue, void *ptr_retrieve );

///
/// \brief Destroy a queue
///
/// Destroy a queue, freeing all its elements.
///
/// \param queue Queue to destroy
///
void
sowr_Queue_Destroy( sowr_Queue *queue );

///
/// \brief Destroy a queue
///
/// Destroy a queue created by \a sowr_Queue_CreateS(), freeing all its elements.
///
/// \param queue Queue to destroy
///
void
sowr_Queue_DestroyS( sowr_Queue *queue );

#endif //!SOWR_LIB_CONTAINER_QUEUE_H
