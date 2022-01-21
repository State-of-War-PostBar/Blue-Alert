/*************************************************************************************************
*                                                                                                *
*                                  [ State of War: Remastered ]                                  *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                  A free, open-source software project recreating an old game.                  *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
*                                                                                                *
*    State of War: Remastered is a free software. You can freely do whatever you want with it    *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*  By the time this line is written, the version of the license document is 1, but you may use   *
*                  any later version of the document released by mhtvsSFrpHdE.                   *
*                                                                                                *
*     State of War: Remastered is created, intended to be useful, but without any warranty.      *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*              <https://github.com/State-of-War-PostBar/blrt/blob/master/LICENSE>.               *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                        <https://github.com/State-of-War-PostBar/blrt>.                         *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                               Mission is successfully completed.                               *
*                                                                                                *
**************************************************************************************************/

#ifndef BLRT_LIB_CONTAINER_STACK_H
#define BLRT_LIB_CONTAINER_STACK_H

#include <pch.h>

#include "vector.h"

typedef void (*blrt_StackFreeFunc)( void * );
typedef void (*blrt_StackEatFunc)( void * );
typedef bool (*blrt_StackCmpFunc)( const void *, const void * );

typedef blrt_Vector blrt_Stack;

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
blrt_Stack *
blrt_Stack_Create ( size_t elem_size, blrt_StackFreeFunc free_func );

///
/// \brief Create a stack
///
/// Create a stack by stack.<BR />
/// <B>The created stack must be freed by \a blrt_Stack_DestroyS().</B>
///
/// \param elem_size Size of stack's elements
/// \param free_func Function to call when the stack frees an element
///
/// \return Created stack
///
blrt_Stack
blrt_Stack_CreateS ( size_t elem_size, blrt_StackFreeFunc free_func );

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
blrt_Stack_Last( const blrt_Stack *stack );

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
blrt_Stack_Find( const blrt_Stack *stack, const void *data, blrt_StackCmpFunc cmp );

///
/// \brief Expand the stack
///
/// Expand the stack, usually doubling its capacity unless the length is 0.
///
/// \param stack Stack to expand
///
void
blrt_Stack_Expand( blrt_Stack *stack );

///
/// \brief Expand the stack
///
/// Expand the stack until its capacity reaches the set limit.
///
/// \param stack Stack to expand
/// \param size Target size for expanding
///
void
blrt_Stack_ExpandUntil( blrt_Stack *stack, size_t size );

///
/// \brief Eat the stack
///
/// Eat the elements of the stack. After the operations all elements will be freed.
///
/// \param stack Stack to eat
/// \param func Function for eating
///
void
blrt_Stack_Eat( blrt_Stack *stack, blrt_StackEatFunc func );

///
/// \brief Clear out a stack
///
/// Clear the content of a stack. Its capacity is unchanged.
///
/// \param stack Stack to clear
///
void
blrt_Stack_Clear( blrt_Stack *stack );

///
/// \brief Shrink the stack
///
/// Shrink the stack to just enough to fit its contents.
///
/// \param stack Stack to shrink
///
void
blrt_Stack_ShrinkToFit( blrt_Stack *stack );

///
/// \brief Push an element
///
/// Push an element into the last of stack.
///
/// \param stack Stack to push into
/// \param elem Element to be pushed
///
void
blrt_Stack_Push( blrt_Stack *stack, const void *elem );

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
blrt_Stack_Pop( blrt_Stack *stack, void *ptr_retrieve );

///
/// \brief Destroy a stack
///
/// Destroy a stack, freeing all its elements.
///
/// \param stack Stack to destroy
///
void
blrt_Stack_Destroy( blrt_Stack *stack );

///
/// \brief Destroy a stack
///
/// Destroy a stack created by \a blrt_Stack_CreateS(), freeing all its elements.
///
/// \param stack Stack to destroy
///
void
blrt_Stack_DestroyS( blrt_Stack *stack );

#endif //!BLRT_LIB_CONTAINER_STACK_H
