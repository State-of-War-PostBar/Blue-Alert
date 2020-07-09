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

#ifndef SOWR_LIB_CONTAINER_STACK_H
#define SOWR_LIB_CONTAINER_STACK_H

#include <pch.h>

#include "vector.h"

typedef void (*sowr_StackFreeFunc)( void * );
typedef void (*sowr_StackEatFunc)( void * );
typedef bool (*sowr_StackCmpFunc)( const void *, const void * );

typedef sowr_Vector sowr_Stack;

///
/// \brief Create a stack
///
/// Create a stack.
///
/// \param elem_size Size of stack's elements
/// \param free_func Function to call when the stack frees an element
///
/// \return Created stack
///
sowr_Stack *
sowr_Stack_Create ( size_t elem_size, sowr_StackFreeFunc free_func );

///
/// \brief Create a stack
///
/// Create a stack by stack.<BR />
/// <B>The created stack must be freed by \a sowr_Stack_DestroyS().</B>
///
/// \param elem_size Size of stack's elements
/// \param free_func Function to call when the stack frees an element
///
/// \return Created stack
///
sowr_Stack
sowr_Stack_CreateS ( size_t elem_size, sowr_StackFreeFunc free_func );

///
/// \brief Get the last element
///
/// Get the last element of the stack.
///
/// \param stack Stack
///
/// \return Pointer to the last element
///
void *
sowr_Stack_Last( const sowr_Stack *stack );

///
/// \brief Find an element in the stack
///
/// Search for an element in the given stack.
///
/// \param stack Stack
/// \param data Data to search
/// \param cmp Comparision function
///
/// \return Pointer to the found element, NULL otherwise.
///
void *
sowr_Stack_Find( const sowr_Stack *stack, const void *data, sowr_StackCmpFunc cmp );

///
/// \brief Expand the stack
///
/// Expand the stack, usually doubling its capacity unless the length is 0.
///
/// \param stack Stack to expand
///
void
sowr_Stack_Expand( sowr_Stack *stack );

///
/// \brief Expand the stack
///
/// Expand the stack until its capacity reaches the set limit.
///
/// \param stack Stack to expand
/// \param size Target size for expanding
///
void
sowr_Stack_ExpandUntil( sowr_Stack *stack, size_t size );

///
/// \brief Eat the stack
///
/// Eat the elements of the stack. After the operations all elements will be freed.
///
/// \param stack Stack to eat
/// \param func Function for eating
///
void
sowr_Stack_Eat( sowr_Stack *stack, sowr_StackEatFunc func );

///
/// \brief Clear out a stack
///
/// Clear the content of a stack. Its capacity is unchanged.
///
/// \param stack Stack to clear
///
void
sowr_Stack_Clear( sowr_Stack *stack );

///
/// \brief Shrink the stack
///
/// Shrink the stack to just enough to fit its contents.
///
/// \param stack Stack to shrink
///
void
sowr_Stack_ShrinkToFit( sowr_Stack *stack );

///
/// \brief Push an element
///
/// Push an element into the last of stack.
///
/// \param stack Stack to push into
/// \param elem Element to be pushed
///
void
sowr_Stack_Push( sowr_Stack *stack, const void *elem );

///
/// \brief Pop the last element of stack
///
/// Pop the last element of stack into the pointer.
/// Whether ptr_retrieve is NULL or not, the last element will be overriden.
///
/// \param stack Stack to pop out
/// \param ptr_retrieve Pointer to retrieve the result
///
void
sowr_Stack_Pop( sowr_Stack *stack, void *ptr_retrieve );

///
/// \brief Destroy a stack
///
/// Destroy a stack, freeing all its elements.
///
/// \param stack Stack to destroy
///
void
sowr_Stack_Destroy( sowr_Stack *stack );

///
/// \brief Destroy a stack
///
/// Destroy a stack created by \a sowr_Stack_CreateS(), freeing all its elements.
///
/// \param stack Stack to destroy
///
void
sowr_Stack_DestroyS( sowr_Stack *stack );

#endif //!SOWR_LIB_CONTAINER_STACK_H
