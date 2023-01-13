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

#ifndef BLRT_LIB_CONTAINER_LINKED_LIST_H
#define BLRT_LIB_CONTAINER_LINKED_LIST_H

#include <pch.h>

typedef void (*blrt_LinkedListFreeFunc)( void * );
typedef void (*blrt_LinkedListWalkFunc)( void * );
typedef bool (*blrt_LinkedListCmpFunc)( const void *, const void * );

typedef struct blrt_LinkedListNode
{
    size_t data_size;
    void *data;
    struct blrt_LinkedListNode *next;
} blrt_LinkedListNode;

typedef struct blrt_LinkedList
{
    size_t length;
    blrt_LinkedListFreeFunc free_func;
    blrt_LinkedListNode head;
} blrt_LinkedList;

///
/// \brief Create a linked list
///
/// Create a linked list ready to use.
///
/// \param free_func Function to call when freeing an element
///
/// \return Created linked list
///
blrt_LinkedList *
blrt_LinkedList_Create( blrt_LinkedListFreeFunc free_func );

///
/// \brief Create a linked list
///
/// Create a linked list ready to use by stack.<BR />
/// <B>The created list must be destroyed by \a blrt_LinkedList_DestroyS().</B>
///
/// \param free_func Function to call when freeing an element
///
/// \return Created linked list
///
blrt_LinkedList
blrt_LinkedList_CreateS( blrt_LinkedListFreeFunc free_func );

///
/// \brief Walk a linked list
///
/// Walk a linked list with given function.
///
/// \param list List to walk
/// \param func Function for walking
///
void
blrt_LinkedList_Walk( blrt_LinkedList *list, blrt_LinkedListWalkFunc func );

///
/// \brief Clear a linked list
///
/// Clear the contents of a linked list, freeing all its contents.
///
/// \param list List to clear
///
void
blrt_LinkedList_Clear( blrt_LinkedList *list );

///
/// \brief Insert data to list
///
/// Insert some data to the list
///
/// \param list List to insert
/// \param data_size Size of data
/// \param data Data to insert
///
void
blrt_LinkedList_Insert( blrt_LinkedList *list, size_t data_size, const void *data );

///
/// \brief Pop the first element
///
/// Pop the first element of the list.<BR />
/// If the given retrieving pointer is NULL, the data is freed.
///
/// \param list List to pop
/// \param ptr_retrieve Pointer to retrieve the element if needed
///
void
blrt_LinkedList_Pop( blrt_LinkedList *list, void *ptr_retrieve );

///
/// \brief Find an element
///
/// Find an element in the linked list, only look for the first one matches.
///
/// \param list List to search
/// \param data Data to find
/// \param func Function for comparision
///
/// \return The node containing the element if found, NULL otherwise.
///
blrt_LinkedListNode *
blrt_LinkedList_Find( const blrt_LinkedList *list, const void *data, blrt_LinkedListCmpFunc func );

///
/// \brief Extract elements
///
/// Extract elements in the list.<BR />
/// If the given retrieving pointer is NULL, the data is freed.<BR />
/// This function will extract out all the elements matching the comparision, but only copying once.
///
/// \param list List to operate
/// \param data Data to find
/// \param func Function for comparision
/// \param ptr_retrieve Pointer to retrieve the data if needed
///
/// \return Successfully extracted elements count
///
size_t
blrt_LinkedList_Take( blrt_LinkedList *list, const void *data, blrt_LinkedListCmpFunc func, void *ptr_retrieve );

///
/// \brief Destroy a linked list
///
/// Destroy a linked list.
///
/// \param list List to destroy
///
void
blrt_LinkedList_Destroy( blrt_LinkedList *list );

///
/// \brief Destroy a linked list
///
/// Destroy a linked list created by \a blrt_LinkedList_CreateS().
///
/// \param list List to destroy
///
void
blrt_LinkedList_DestroyS( blrt_LinkedList *list );

#endif //!BLRT_LIB_CONTAINER_LINKED_LIST_H
