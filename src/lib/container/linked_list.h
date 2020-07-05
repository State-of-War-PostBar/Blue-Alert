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

#ifndef SOWR_LIB_CONTAINER_LINKED_LIST_H
#define SOWR_LIB_CONTAINER_LINKED_LIST_H

#include <pch.h>

typedef void (*sowr_LinkedListFreeFunc)( void * );
typedef void (*sowr_LinkedListWalkFunc)( void * );
typedef bool (*sowr_LinkedListCmpFunc)( const void *, const void * );

typedef struct sowr_LinkedListNode
{
    size_t data_size;
    void *data;
    struct sowr_LinkedListNode *next;
} sowr_LinkedListNode;

typedef struct sowr_LinkedList
{
    size_t length;
    sowr_LinkedListFreeFunc free_func;
    sowr_LinkedListNode *head;
} sowr_LinkedList;

///
/// \brief Create a linked list
///
/// Create a linked list ready to use.
///
/// \param free_func Function to call when freeing an element
///
/// \return Created linked list
///
sowr_LinkedList *
sowr_LinkedList_Create( sowr_LinkedListFreeFunc free_func );

///
/// \brief Create a linked list
///
/// Create a linked list ready to use by stack.<BR />
/// <B>The created list must be destroyed by \a sowr_LinkedList_Destroy().</B>
///
/// \param free_func Function to call when freeing an element
///
/// \return Created linked list
///
sowr_LinkedList
sowr_LinkedList_CreateS( sowr_LinkedListFreeFunc free_func );

///
/// \brief Walk a linked list
///
/// Walk a linked list with given function.
///
/// \param list List to walk
/// \param func Function for walking
///
void
sowr_LinkedList_Walk( sowr_LinkedList *list, sowr_LinkedListWalkFunc func );

///
/// \brief Clear a linked list
///
/// Clear the contents of a linked list, freeing all its contents.
///
/// \param list List to clear
///
void
sowr_LinkedList_Clear( sowr_LinkedList *list );

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
sowr_LinkedList_Insert( sowr_LinkedList *list, size_t data_size, const void *data );

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
sowr_LinkedList_Pop( sowr_LinkedList *list, void *ptr_retrieve );

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
sowr_LinkedListNode *
sowr_LinkedList_Find( const sowr_LinkedList *list, const void *data, sowr_LinkedListCmpFunc func );

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
sowr_LinkedList_Take( sowr_LinkedList *list, const void *data, sowr_LinkedListCmpFunc func, void *ptr_retrieve );

///
/// \brief Destroy a linked list
///
/// Destroy a linked list.
///
/// \param list List to destroy
///
void
sowr_LinkedList_Destroy( sowr_LinkedList *list );

///
/// \brief Destroy a linked list
///
/// Destroy a linked list created by \a sowr_LinkedList_CreateS().
///
/// \param list List to destroy
///
void
sowr_LinkedList_DestroyS( sowr_LinkedList *list );

#endif //!SOWR_LIB_CONTAINER_LINKED_LIST_H
